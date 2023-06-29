#include "Ft_irc.hpp"

std::string	generateMsgPriv(User *userX, std::string	*params, int paramNumber, int flag)
{
	int			i;
	std::string	cmd;

	i = 2;
	if (flag == PRIVMSG)
		cmd = "PRIVMSG";
	else if (flag == NOTICE)
		cmd = "NOTICE";
	else
		cmd = "QUIT";
	std::string	privMsg = ":" + userX->getNick() + "!~" + userX->getUsrName() + "@" + userX->getUsrHostName() + " " + cmd + " " + params[1] + " ";
	if (params[2][0] == ':')
		privMsg += params[2] + "\r\n";
	else
	{
		privMsg += ":";
		while (i < paramNumber + 1)
		{
			privMsg += params[i] + " ";
			i++;
		}
		privMsg += "\r\n";
	}
	return (privMsg);
}

void Server::handleCmdMsg(std::string	*params, User *userX ,int paramNumber, int flag)
{
	int			fd;
	std::string	*paramsRep;
	Channel		*channelName;
	std::string	privMsg;
	//nick part
	if (paramNumber < 2)
	{
		paramsRep = allocateForParams(2);
		paramsRep[0] = userX->getNickForReply();
		paramsRep[1] = ":No recipient given";
		sendReply(userX->sendFd, ERR_NORECIPIENT, paramsRep);
		delete[] paramsRep;
	}
	else if (params[1][0] != '#')
	{
		if (params[2].find("DCC SEND") != std::string::npos)
		{
			//this->dcc(params[2]);
			// std::string	msg = "NOTICE styes :DCC SEND accepted for file init_env.c\r\n";
			// if (send(fd, msg.c_str(), msg.length(), 0) == -1)
			// 		throw errorErrno();
			;
		}
		else
		{
			fd =  this->findClient(params[1]);
			if (fd == -1)
			{
					paramsRep = allocateForParams(2);
					paramsRep[0] = userX->getNickForReply();
					paramsRep[1] = ":No such nick";
					sendReply(userX->sendFd, ERR_NOSUCHNICK, paramsRep);
					delete[] paramsRep;
			}
			else
			{
				privMsg = generateMsgPriv(userX, params, paramNumber, flag);
				correspondence(CLIENT_TO_SERVER, privMsg);
				if (send(fd, privMsg.c_str(), privMsg.length(), 0) == -1)
					throw errorErrno();
			}
		}
	}
	/*
	* channel part
	* check if user belong to channel
	*/
	else
	{
		privMsg = generateMsgPriv(userX, params, paramNumber, flag);
		channelName = channelFinder(params[1]);
		if (channelName == NULL)
		{
			paramsRep = allocateForParams(2);
			paramsRep[0] = userX->getNickForReply();
			paramsRep[1] = ":No such channel";
			sendReply(userX->sendFd, ERR_NOSUCHNICK, paramsRep);
		}
		else if (channelName->findUserinChannel(userX->sendFd) != -1) //can't find in chnnel
		{
			for (int i = 0; i < channelName->channelMembers.size(); i++)
			{
				if (channelName->channelMembers[i] != userX)
				{
					if (send(channelName->channelMembers[i]->sendFd, privMsg.c_str(), privMsg.length(), 0) == -1)
						throw errorErrno();
				}
			}
		}
		else
		{
			paramsRep = allocateForParams(2);
			paramsRep[0] = userX->getNickForReply();
			paramsRep[1] = ":Cannot send to channel";
			sendReply(userX->sendFd, ERR_CANNOTSENDTOCHAN, paramsRep);
		} 
	}
}

int	Server::findClient(std::string	nick)
{
	std::map<int, User*>::iterator 	it;
	int								fd;
	fd = -1;
	for (it = this->users.begin(); it != this->users.end(); ++it)
	{
		if (it->second->getNick() == nick)
			fd = it->first;
	}
	return (fd);
}