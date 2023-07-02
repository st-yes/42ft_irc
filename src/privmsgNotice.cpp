# include "../includes/Ft_irc.hpp"

std::string	Server::generateMsgPriv(User *userX, std::string	*params, int paramNumber, int flag)
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
	params[2] =  params[2].substr(0, params[2].find_last_of(" "));
	privMsg += params[2] + "\r\n";
	return (privMsg);
}


std::string	Server::generateMsgPrivNc(User *userX, std::string	nick, std::string	msg, int flag)
{
	std::string	cmd = "PRIVMSG";
	if (flag == NOTICE)
		cmd = "NOTICE";
	std::string	privMsg = ":" + userX->getNick() + "!~" + userX->getUsrName() + "@" + userX->getUsrHostName() + " " + cmd + " " + nick + " ";
	msg =  msg.substr(0, msg.find_last_of(" "));
	privMsg += msg + "\r\n";
	return (privMsg);
}
void Server::handleCmdMsg(std::string	*params, User *userX ,int paramNumber, int flag)
{
	int			fd;
	std::string	*paramsRep;
	Channel		*channelName;
	std::string	privMsg;
	std::vector<User*>	senders;
	senders.push_back(userX);
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
			fd =  this->findClient(params[1]);
			if (fd == -1)
			{
					this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHNICK, "no such nick"), senders);
			}
			else
			{
				privMsg = generateMsgPriv(userX, params, paramNumber, flag);
				correspondence(CLIENT_TO_SERVER, privMsg);
				if (send(fd, privMsg.c_str(), privMsg.length(), 0) == -1)
					throw errorErrno();
			}
	}
	else
	{
		privMsg = generateMsgPriv(userX, params, paramNumber, flag);
		channelName = channelFinder(params[1]);
		if (channelName == NULL)
		{

			this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHNICK, "no such channel"), senders);
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

			this->sendHermes(this->sendNumericCode(userX, NULL, ERR_CANNOTSENDTOCHAN, "cant send to channel"), senders);
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

void	Server::handleMultiMsg(std::string	*params, User *userX ,int paramNumber, int flag)
{
	int			fd;
	std::string	*paramsRep;
	Channel		*channelName;
	std::string	privMsg;
	std::vector<User*>	senders;
	senders.push_back(userX);

	std::cout << "multiple here = " << params[2] << std::endl;
	if (paramNumber != 2)
	{
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "syntax error"), senders);
	}
	else
	{
		std::istringstream	iss(params[1]);
		std::string			token;
		while (std::getline(iss, token,  ','))
		{
			if (token[0] != '#')
			{
					fd =  this->findClient(token);
					if (fd == -1)
					{
							this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHNICK, "no such nick"), senders);
					}
					else
					{
						privMsg = generateMsgPrivNc(userX, token, params[2], flag);
						correspondence(CLIENT_TO_SERVER, privMsg);
						if (send(fd, privMsg.c_str(), privMsg.length(), 0) == -1)
							throw errorErrno();
					}
			}
			else
			{
				privMsg = generateMsgPrivNc(userX, token, params[2], flag);
				channelName = channelFinder(token);
				if (channelName == NULL)
				{
					this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHNICK, "no such channel"), senders);
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
					this->sendHermes(this->sendNumericCode(userX, NULL, ERR_CANNOTSENDTOCHAN, "cant send to channel"), senders);
				} 
			}
		}
	}
}