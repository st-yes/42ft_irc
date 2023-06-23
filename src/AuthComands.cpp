#include "../includes/Ft_irc.hpp"

/*----CMDS: PASS, NICK, USER */

/*
* if PASS not given OR incorrect: ERR_PASSWDMISMATCH
* will i use ERR_NEEDMOREPARAMS ?
*/
void	Server::handleCmdPass(std::string	*params, User *userX ,int paramNumber)
{
	std::string	*paramsRep;
	std::map<int, User*>::iterator it;

	if (userX->primer != 3)
	{
		paramsRep = allocateForParams(1);
		paramsRep[0] = ":invalid authentication order";
		sendReply(userX->sendFd,"banana", ERR_PASSWDMISMATCH, paramsRep);
		delete[] paramsRep;
	}
	else if (paramNumber > 1 || !this->passCorrect(params[1]))
	{
		paramsRep = allocateForParams(2);
		paramsRep[0] = "*";
		paramsRep[1] = ":invalid password";
		sendReply(userX->sendFd,"banana", ERR_PASSWDMISMATCH, paramsRep);
		delete[] paramsRep;
	}
	else if (paramNumber < 1)
	{
		paramsRep = allocateForParams(2);
		paramsRep[0] = "PASS";
		paramsRep[1] = ":need more parameters";
		sendReply(userX->sendFd,"banana", ERR_NEEDMOREPARAMS, paramsRep);
		delete[] paramsRep;
	}
}


void	Server::handleCmdNick(std::string	*params, User *userX ,int paramNumber)
{
	int		fd;
	std::string	*paramsRep;
	std::string	nickname;

	;
}

void	Server::handleCmdUser(std::string	*params, User *userX ,int paramNumber)
{
	std::string	*paramsRep;
	
	userX->userAuthentified = true;
	this->sendWelcome(userX);
}