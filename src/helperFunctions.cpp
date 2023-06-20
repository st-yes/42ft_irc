#include "Ft_irc.hpp"


bool	Server::passCorrect(std::string passUser)
{
	if (passUser == this->passWord)
		return (true);
	return (false);
}

bool	Server::nickAlreadyInUse(std::string nick)
{
	std::map<int, User *>::iterator it;

	for (it = this->users.begin(); it != this->users.end(); ++it)
	{
		if (it->second->userNick == nick)
			return (true);
	}
	return (false);
}

int	Server::findClientFd(User *userX)
{
	std::map<int, User*>::iterator 	it;
	int								fd = -1;
	for (it = users.begin(); it != users.end(); ++it)
	{
		if (it->second == userX)    // Access the value
			{
				fd = it->first;
				break;
			}
	}
	return (fd);
}

// void	alternatives(std::string command, std::string input)
// {
// 	std::string	commandUP = // all uppercase
// 	std::string	commandlower = //all lowecase 
// }