#include "Ft_irc.hpp"

/*----CMDS: PASS, NICK, USER */

/*
* if PASS not given OR incorrect: ERR_PASSWDMISMATCH
* will i use ERR_NEEDMOREPARAMS ?
*/
void	Server::handleCmdPass(std::string	*params, User *userX ,int paramNumber)
{
	int fd = -1;
	std::string	*paramsRep;
	std::map<int, User*>::iterator it;

	fd = this->findClientFd(userX);
	if (userX->primer != 3)
	{
		paramsRep = allocateForParams(1);
		paramsRep[0] = ":invalid authentication order";
		sendReply(fd,"banana", ERR_PASSWDMISMATCH, paramsRep);
		delete[] paramsRep;
	}
	else if (paramNumber > 1 || !this->passCorrect(params[1]))
	{
		paramsRep = allocateForParams(2);
		paramsRep[0] = "*";
		paramsRep[1] = ":invalid password";
		sendReply(fd,"banana", ERR_PASSWDMISMATCH, paramsRep);
		delete[] paramsRep;
	}
	else if (paramNumber < 1)
	{
		paramsRep = allocateForParams(2);
		paramsRep[0] = "PASS";
		paramsRep[1] = ":need more parameters";
		sendReply(fd,"banana", ERR_NEEDMOREPARAMS, paramsRep);
		delete[] paramsRep;
	}
}

// /*
// * 
// */
void	Server::handleCmdNick(std::string	*params, User *userX ,int paramNumber)
{
	int		fd;
	std::string	*paramsRep;
	std::string	nickname;

	fd  = findClientFd(userX);
	;
}

void	Server::handleCmdUser(std::string	*params, User *userX ,int paramNumber)
{
	int j;
	int fd;
	std::string	*paramsRep;
	
	fd = this->findClientFd(userX);
	userX->userAuthentified = true;
	this->sendWelcome(fd, userX);
	;
}