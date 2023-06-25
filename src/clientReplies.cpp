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
	{
		User* user = this->findUserinServ(clientFd);
		this->lostConnection(user);
		return;
	}
	std::cout << "here homie ::: " << reply << std::endl;
	//this->lostConnection(clientFd, k);
}

void Server::sendReply(int clientFd, std::string numericCode, std::string *params)
{
	std::string	reply;
	int			i;

	reply = numericCode;
	i = 0;
	while (params[i] != "")
	{
		reply += " ";
		reply += params[i];
		i++;
	}
	reply += "\r\n";
	//std::cout << "[client -> server]" << reply << std::flush;
	//correspondence(CLIENT_TO_SERVER, reply);
	if (send(clientFd, reply.c_str(), reply.length(), 0) == -1)
		throw errorErrno(); // check for err
}

void Server::sendGenericReply(User *userX, std::string prefix, Channel *chan){
	std::string reply;
	reply = ":" + userX->getNickForReply() + " " + prefix + " " + chan->channelName + "\r\n";
	std::cout << "---->" << reply << std::endl;
	if (send(userX->sendFd, reply.c_str(), reply.length(), 0) == -1)
	{
		this->lostConnection(userX);
		return ;
	}
}

/* connection established */
void	Server::sendWelcome(User *user)
{
	if (!user->userAuthentified)
		return;
	std::string	msg = "";

	msg += std::string(RPL_WELCOME) + " " + user->getNickForReply() + " :Welcome to BANANA TASBA7 " + user->getNickForReply() + "!" + user->getUsrName() + "@" + user->getUsrHostName() + "\r\n";
	//correspondence(CLIENT_TO_SERVER, msg);
	if (send(user->sendFd, msg.c_str(), msg.length(), 0) == -1)
		throw errorErrno(); // check for err
	this->defaultChannelsAdd(user);

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
void User::getCommands(std::string buffer, bool reset)
{
	if (reset)
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
