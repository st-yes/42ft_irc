#include "../includes/Ft_irc.hpp"

void Server::handleCmdMsg(std::string	*params, User *userX ,int paramNumber)
{
	int			fd;
	std::string	*paramsRep;
	Channel		*channelName;
	std::vector<User*> sender;
	sender.push_back(userX);
	//channel
	if (params[1][0] != '#')
	{
		fd =  this->findClient(params[1]);
		if (fd == -1)
		{
				// paramsRep = allocateForParams(2);
				// paramsRep[0] = userX->getNickForReply();
				// paramsRep[1] = ":No such nick";
				// sendReply(userX->sendFd, ERR_NOSUCHNICK, paramsRep);
				// delete[] paramsRep;
				this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHNICK, "there is no such nick"), sender);
		}
		else
		{
			//std::string msg = ":"+ userX->getNick() + " " + params[1] + " " + params[2] + "\r\n";
			std::string	privMsg = ":" + userX->getNick() + "!~" + userX->getUsrName() + "@" + userX->getUsrHostName() + " " "PRIVMSG" + " " + params[1] + " " + params[2] + "\r\n";
			correspondence(CLIENT_TO_SERVER, privMsg);
			if (send(fd, privMsg.c_str(), privMsg.length(), 0) == -1)
				throw errorErrno();
		}
	}
	//nick
	else
	{
		std::string	privMsg = ":" + userX->getNick() + "!~" + userX->getUsrName() + "@" + userX->getUsrHostName() + " " "PRIVMSG" + " " + params[1] + " " + params[2] + "\r\n";
		channelName = channelFinder(params[1]);
		if (channelName == NULL)
		{
			// paramsRep = allocateForParams(2);
			// paramsRep[0] = userX->getNickForReply();
			// paramsRep[1] = ":No such channel";
			// sendReply(userX->sendFd, ERR_NOSUCHNICK, paramsRep);
			this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHCHANNEL, "theres no such channel"), sender);
		}
		else
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
	}
}
	//find user


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