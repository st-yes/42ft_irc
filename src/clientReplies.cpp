#include "Ft_irc.hpp"

/*-- 
* template try out of server reply 
* :prefix numeric code param1 param2 ... + \r\n 
--*/
void Server::sendReply(int clientFd, std::string prefix, std::string numericCode, std::string *params)
{
	std::string	reply;
	int			i;

	reply = ":" + prefix + " " + numericCode;
	i = 0;
	while (params[i] != "")
	{
		reply += " ";
		reply += params[i];
		i++;
	}
	reply += "\r\n";
	std::cout << "[client -> server]" << reply << std::flush;
	if (send(clientFd, reply.c_str(), reply.length(), 0) == -1)
		throw errorErrno(); // recheck for error
	//this->lostConnection(clientFd, k);
}

/* connection established */
void	Server::sendWelcome(User *user)
{
	std::string	msg;

	msg = ":" + user->serverName + " " + RPL_WELCOME + " " + user->getNick() + " :Welcome to BANANA TASBA7 " + user->getNick() + "!" + user->getUsrName() + "@" + user->getUsrHostName() + "\r\n";
	if (send(user->sendFd, msg.c_str(), msg.length(), 0) == -1)
		throw errorErrno(); // check for error
    msg = user->getNick() + " has joined the server!\r\n"; //message send to all those connected to socket ??
    for (int i = 0; i < this->pollers.size(); i++)
	{
        if (this->pollers[i].fd == this->servSocketFd || this->pollers[i].fd == user->sendFd)
            continue;
		if (authenticated(this->pollers[i].fd))
			if (send(this->pollers[i].fd, msg.c_str(), msg.length(), 0) == -1)
				throw errorErrno(); // Check for error
    }
}

/* even without "\r\n" it get sent to official client */
void	Server::sendInstructions(int clientFd)
{
	std::string	instruct;
	instruct = "if you are trying to connect from a non official irc client\nPlease provide:\nP A S S\nN I C K\nU S E R\r\n";
	if (send(clientFd, instruct.c_str(), instruct.length(), 0) == -1)
		throw errorErrno();
}

/*
* fill the vector commandfull with the buffer content
* equalize nc buffer with IRSSI buffer by adding '\r\n' to buffer's nc
*/
void User::getCommands(std::string buffer)
{
	this->commandFull.clear();

	if (buffer.empty())	
		return;

	std::string singleCommand;
	for (std::size_t i = 0; i < buffer.length(); i++)
	{
		if (buffer[i] == '\r')
		{
			this->commandFull.push_back(singleCommand);
			singleCommand = "";
		}
		else
		{
			if (buffer[i] != '\n')
				singleCommand += buffer[i];
		}
	}
	if (!singleCommand.empty())
	{
		this->commandFull.push_back(singleCommand);
	}
}
