#include "Ft_irc.hpp"

/*----CMDS: PASS, NICK, USER */

/*
* if PASS not given OR incorrect: ERR_PASSWDMISMATCH
* will i use ERR_NEEDMOREPARAMS ?
*/
// void	Server::handleCmdPass(std::string	*params, User *userX ,int paramNumber)
// {
// 	std::string	*paramsRep;
// 	std::map<int, User*>::iterator it;

// 	if (userX->primer != 3)
// 	{
// 		std::cout << "damdam" << std::endl;
// 		paramsRep = allocateForParams(1);
// 		paramsRep[0] = ":invalid authentication order";
// 		sendReply(userX->sendFd,"banana", ERR_PASSWDMISMATCH, paramsRep);
// 		delete[] paramsRep;
// 	}
// 	else if (paramNumber > 1 || !this->passCorrect(params[1]))
// 	{
// 		std::cout << "damdam2" << std::endl;
// 		paramsRep = allocateForParams(2);
// 		paramsRep[0] = "*";
// 		paramsRep[1] = ":invalid password";
// 		sendReply(userX->sendFd,"banana", ERR_PASSWDMISMATCH, paramsRep);
// 		delete[] paramsRep;
// 	}
// 	else if (paramNumber < 1)
// 	{
// 		std::cout << "damdam3" << std::endl;
// 		paramsRep = allocateForParams(2);
// 		paramsRep[0] = "PASS";
// 		paramsRep[1] = ":need more parameters";
// 		sendReply(userX->sendFd,"banana", ERR_NEEDMOREPARAMS, paramsRep);
// 		delete[] paramsRep;
// 	}
// }

// /*
// * 
// */


// void	Server::handleCmdUser(std::string	*params, User *userX ,int paramNumber)
// {
// 	int j;
// 	std::string	*paramsRep;
// 	std::cout <<  "woooooo" <<userX->userAuthentified << std::endl;
// 	userX->userAuthentified = true;
// 	std::cout << "here <" << std::endl;
// 	this->sendWelcome(userX);
// 	;
// }

std::string	assembleParmsIntoToken(std::string * params)
{
	int			i;
	std::string	token;

	i = 0;
	while (params[i] != "")
	{
		token += params[i];
		if (params[i + 1] != "")
			token += " ";
		i++;
	}
	return (token);
}

void	Server::handleCmdNickAgain(int clientFd, std::string	*params, User *userX ,int paramNumber)
{
	std::string	*paramsRep;

	if (nickAlreadyInUse(params[1]))
	{
		userX->oldNick = params[1];
		paramsRep = allocateForParams(3);
		paramsRep[0] = userX->getNickForReply();
		paramsRep[1] = params[1];
		paramsRep[2] = ":Nickname is already in use";
		sendReply(clientFd, ERR_NICKNAMEINUSE, paramsRep);
		delete[] paramsRep;
		userX->userAuthentified = NICK_AGAIN;
	}
	else
	{
		userX->setNick(params[1]);
		this->sendStatusUpdate(clientFd, userX, "NICK", params[1]);
		userX->userAuthentified = false;
		//userX->oldNick = userX->userNick;
		//std::cout << "primer = " << userX->primer << std::endl;
		if (userX->primer == 3 * 5 * 7 && userX->userAuthentified == false && userX->passSet)
		{
			userX->userAuthentified = true;
			this->sendWelcome(userX);
			std::cout << " the user is :::::: " << userX->getNickForReply() << std::endl;
		}
	}
}

void	Server::handleCmdPing(std::string	*params, User *userX ,int paramNumber)
{
	std::string	*reply;
	std::string	token;
	std::string	pongMsg;
	int			i;
	int			clientFd;
	bool		err = false;

	i = 1;
	if (paramNumber < 1)
	{
		reply = allocateForParams(3);
		reply[0] = userX->getNickForReply();
		reply[1] = "PING";
		reply[2] = ":Not enough parameters ";
		this->sendReply(userX->sendFd, ERR_NEEDMOREPARAMS, reply);
		err = true;
	}
	while (params[i] != "")
	{
		token += params[i];
		if (params[i + 1] != "")
			token += " ";
		i++;
	}
	if (!err)
	{
		pongMsg = std::string("PONG") + " " + this->serverName + " " + token;
		//correspondence(CLIENT_TO_SERVER, pongMsg);
		if (send(userX->sendFd, pongMsg.c_str(), pongMsg.length(), 0) == -1)
			throw errorErrno();
	}
}