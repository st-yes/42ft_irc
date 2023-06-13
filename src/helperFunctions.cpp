#include "everyThing.hpp"

/*-----------------------------PARSING-FUNCTIONS------------------------*/
bool    parse_args(char **s)
{
    if (!validPort(s[1]))
    {
        errorMsg(PORT);
        return (false);
    }
    return (true);
}

bool	validPort(std::string s)
{
	int	len;

	len = s.size();
	for (int i = 0; i < len; i++)
		if (!std::isdigit(s[i]))
			return (false);
	if (std::atoi(s.c_str()) < MIN_PORT ||std::atoi(s.c_str()) > MAX_PORT)
		return (false);
	return (true);
}

bool	validPass(std::string s)
{
	int	len;

	len = s.size();
	for (int i = 0; i < len; i++)
		if (!std::isdigit(s[i]))
			return (false);
	return (true);
}

/*------------------------------ERROR-FUNCTIONS-------------------------*/

void	errorMsg(int error)
{
	std::cerr << "ERROR: ";
	if (error == USAGE)
		std::cerr << "usage: ./ircserv PORT PASSWORD" << std::endl;
	else if (error == PASSWORD)
		std::cerr << "invalid password" << std::endl;
	else if (error == PORT)
		std::cerr << "invalid port number" << std::endl;
}

void	errnoCheck(std::string	func)
{
	std::cerr << "ERROR ";
	std::cerr << func << ": " << strerror(errno);
	exit(EXIT_FAILURE);
}

bool has_only_spaces(const std::string &str)
{
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if (!isspace(*it))
			return false;
    }
    return true;
}

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

// void	parseNick(User *newUser, std::string buffer, int start)
// {
// 	int j;

// 	i = 0;
// 	j = start;
	
// 	while (j < buffer.length() && isspace(buffer[j]))
// 		j++;
// 	while (j < buffer.length())
// 	{
// 		if (isspace(buffer[j]))
// 			break;
// 		newUser->nick += buffer[j++];
// 	}
// }

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