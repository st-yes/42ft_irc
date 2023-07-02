#include "../includes/Ft_irc.hpp"

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
	std::vector<User*> send;
	send.push_back(userX);

	if (paramNumber < 1)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_ERRONEUSNICKNAME, "no nickname given"), send);
	}
	else if (paramNumber > 1 || !userX->validNick(params[1]))
	{
		paramsRep = allocateForParams(3);
		paramsRep[0] = userX->getNickForReply();
		paramsRep[1] = assembleParmsIntoToken(&params[1]);
		paramsRep[2] = ":invalid nickname";
		sendReply(userX->sendFd, ERR_ERRONEUSNICKNAME, paramsRep);
		delete[] paramsRep;
		userX->userAuthentified = FAILURE;
	}
	else if (nickAlreadyInUse(params[1]))
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
		userX->oldNick = userX->getNick();
		if (userX->primer == 3 * 5 * 7 && userX->userAuthentified == false && userX->passSet)
		{
			userX->userAuthentified = true;
			this->sendHermes(this->sendNumericCode(userX, NULL, RPL_WELCOME, ""), send);
		}
	}
}

void	Server::handleCmdPing(std::string	*params, User *userX ,int paramNumber)
{
	std::string	*reply;
	std::string	token;
	std::string	pongMsg;
	std::vector<User *> send;
	send.push_back(userX);
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
		pongMsg = std::string("PONG") + " " + this->serverName + " " + token + "\r\n";
		correspondence(CLIENT_TO_SERVER, pongMsg);
		this->sendHermes(pongMsg, send);
	}
}

void	Server::handleCmdRegularNick(std::string	*params, User *userX, int paramNumber)
{
	std::string	*paramsRep;
	std::vector<User *> send;
	send.push_back(userX);

	if (paramNumber < 1)
	{
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_ERRONEUSNICKNAME, "no nickname given"), send);
	}
	else if (paramNumber > 1 || !userX->validNick(params[1]))
	{
		paramsRep = allocateForParams(3);
		paramsRep[0] = userX->getNick();
		paramsRep[1] = assembleParmsIntoToken(&params[1]);
		paramsRep[2] = ":invalid nickname";
		sendReply(userX->sendFd, ERR_ERRONEUSNICKNAME, paramsRep);
		delete[] paramsRep;
	}
	else if (nickAlreadyInUse(params[1]))
	{
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NICKNAMEINUSE, "nickname already in use"), send);
	}
	else
	{
		userX->oldNick = userX->getNick();
		userX->setNick(params[1]);
		this->sendStatusUpdate(userX->sendFd, userX, "NICK", params[1]);
	}
}

