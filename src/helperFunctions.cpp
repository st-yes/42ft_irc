#include "Ft_irc.hpp"


/*------------------------------ERROR-FUNCTIONS-------------------------*/

/* check if string has only spaces */
bool hasOnlySpace(const std::string& str)
{
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if (!isspace(*it))
			return (false);
	}
	return (true);
}

/*-- parse pass and nick --*/
void	parsePassNick(User *newUser, std::string buffer, int start, int comm)
{
	int j;

	j = start;
	
	while (j < buffer.length() && isspace(buffer[j]))
		j++;
	while (j < buffer.length())
	{
		if (isspace(buffer[j]))
			break;
		if (comm == PASS)
			newUser->userPass += buffer[j++];
		else if (comm == NICK)
			newUser->userNick += buffer[j++];
	}
}


/*
* parse USER command
* USER username hostname servername :real name 
*/
bool	parseUser(User *newUser, std::string buffer, int start)
{
	int j;
	int i;
	int part;

	i = 0;
	j = start;
	part = 0;
	while (part < 3 && j < buffer.length())
	{
		while (j < buffer.length() && isspace(buffer[j]))
			j++;
		while (j < buffer.length() && !isspace(buffer[j]))
		{
			if (part == 0)
				newUser->userName += buffer[j];
			else if (part == 1)
				newUser->userHostName += buffer[j];
			else if (part == 2)
				newUser->serverName += buffer[j];
			newUser->userCommand += buffer[j++];
		}
		part++;
	}
	if (part == 3 && j < buffer.length())
	{
		while (j < buffer.length())
		{
			if (buffer[j] == '\n')
				break;
			newUser->userFullName += buffer[j];
			newUser->userCommand += buffer[j++];
		}
		part++;
	}
	if (part == 4 && hasOnlySpace(newUser->userFullName)) //check for : first??
		return (false);
	else if (part == 4)
		return (true);
	return (false);
}

void getCredentials(User *newUser, std::string buffer, int start, int comm)
{
	int j;

	j = start;
	
	while (j < buffer.length() && isspace(buffer[j]))
		j++;
	while (j < buffer.length())
	{
		if (isspace(buffer[j]))
			break;
		if (comm == PASS)
			newUser->userPass += buffer[j++];
		else if (comm == NICK)
			newUser->userNick += buffer[j++];
		else if (comm == USER)
			newUser->userCommand += buffer[j++];
	}
}

bool	Server::passCorrect(std::string passUser)
{
	if (passUser == this->passWord)
		return (true);
	return (false);
}

bool	Server::nickAlreadyInUse(std::string nick, int i)
{
	std::map<int, User *>::iterator it;

	for (it = this->users.begin(); it != this->users.end(); ++it)
	{
		if (it->second->userNick == nick && it->first != this->pollers[i].fd)
			return (true);
	}
	return (false);
}