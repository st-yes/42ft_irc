#include "../includes/Ft_irc.hpp"

void Server::sendReply(int clientFd, std::string prefix, std::string numericCode, std::string *params)
{
	std::string	reply;
	int			i;
	User* user = this->findUserinServ(clientFd);
	if (!user)
		return;
	reply = ":" + prefix + " " + numericCode + " " + user->getNickForReply();
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
		this->lostConnection(user);
		return;
	}
}

void Server::sendReply(int clientFd, std::string numericCode, std::string *params)
{
	std::string	reply;
	int			i;

	reply = std::string(":BANANA ") +numericCode;
	i = 0;
	while (params[i] != "")
	{
		reply += " ";
		reply += params[i];
		i++;
	}
	reply += "\r\n";
	if (send(clientFd, reply.c_str(), reply.length(), 0) == -1)
		throw errorErrno();
}

void Server::sendGenericReply(User *userX, std::string prefix, Channel *chan, std::string opt){
	std::string reply;
	std::string option = " " + opt;
	if (prefix == "JOIN" || prefix == "PART" || prefix == "KICK")
		reply = ":" + userX->getNickForReply() + "@" + userX->getHostForReply() + " " + prefix + " " + chan->channelName + "\r\n";
	else if (prefix == "TOPIC")
		reply = ":" + userX->getNickForReply() + " " + prefix + " " + chan->channelName + " " + chan->channelTopic + "\r\n";
	else if (prefix == "MODE")
		reply = ":" + userX->getNickForReply() + "@" + userX->getHostForReply() + " " + prefix + " " + chan->channelName + option + "\r\n";

	std::cout << "---->" << reply << std::endl;
	if (send(userX->sendFd, reply.c_str(), reply.length(), 0) == -1)
	{
		this->lostConnection(userX);
		return ;
	}
	for (int i = 0; i != chan->channelMembers.size(); i++){
		if (chan->channelMembers[i]->sendFd == userX->sendFd)
			continue;
		if (send(chan->channelMembers[i]->sendFd, reply.c_str(), reply.length(), 0) == -1)
		{
			this->lostConnection(userX);
			return ;
		}
	}
	std::cout << "Whats up ::: " << reply << std::endl;
}

void	Server::sendWelcome(User *user)
{
	if (!user->userAuthentified)
		return;
	std::string	msg = "";

	std::string asciiArt = "\\ __                           \n"
                       "/ /  ___ ____  ___ ____  ___ _ \n"
                       "/ _ \\/ _ `/ _ \\/ _ `/ _ \\/ _ `/\n"
                       "/_.__/\\_,_/_//_/\\_,_/_//_/\\_,_/ \n";

	msg += std::string(RPL_WELCOME) + " " + user->getNickForReply() + " :Welcome to\n" + asciiArt + "\r\n";
	if (send(user->sendFd, msg.c_str(), msg.length(), 0) == -1)
		throw errorErrno();
	this->defaultChannelsAdd(user);

}

void	Server::sendInstructions(int clientFd)
{
	std::string	instruct;
	instruct = "if you are trying to connect from a non official irc client\nPlease provide:\nP A S S\nN I C K\nU S E R\r\n";
	if (send(clientFd, instruct.c_str(), instruct.length(), 0) == -1)
		throw errorErrno();
}

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

std::string	Server::sendNumericCode(User *userX, Channel* channel, std::string numericCode, std::string lines){
	std::string reply = "";
	std::string nameChannel = "";
	std::string asciiArt = std::string(BOLD) + "   __                           \n" +
                      std::string(BOLD) + "  / /   ___ ____  ___ ____  ___ _ \n" +
                       std::string(BOLD) + " / _ \\/ _ `/ _ \\/ _ `/ _ \\/ _ `/\n" + 
                      std::string(BOLD) + "/_.__/\\_,_/_//_/\\_,_/_//_/\\_,_/ \n" + std::string(RESET);
	if(channel)
		nameChannel = channel->channelName + " ";
	if (numericCode != RPL_WELCOME && numericCode != RPL_NAMREPLY)
		reply += ":BANANA " + numericCode + " " + userX->getNickForReply() + " " + nameChannel + ":" + lines + "\r\n";
	else if (numericCode == RPL_NAMREPLY){
			reply = std::string(":BANANA ") + numericCode + " " + userX->getNickForReply() + "@" + userX->getUsrHostName() + " = "+ nameChannel + ":";
			for (int i = 0; i != channel->channelMembers.size(); i++){
				reply += channel->channelMembers[i]->getNickForReply() + " ";
			}
			reply += "\r\n";
		}
	else if (numericCode == RPL_WELCOME)
		reply += std::string(RPL_WELCOME) + " " + userX->getNickForReply() + " :Welcome to\n" + asciiArt + "\r\n";
	return reply;
}

std::string	Server::sendGenericCode(User *userX, Channel *chan, std::string prefix, std::string lines){
	std::string reply = ":" + userX->getNickForReply() + "@" + userX->getHostForReply() + " " + prefix + " " + chan->channelName;
	if (prefix == "TOPIC")
		reply += " " + chan->channelTopic + "\r\n";
	else if (prefix == "MODE" || prefix == "KICK" || prefix == "QUIT" || prefix == "PART")
		reply += " " + lines + "\r\n";
	else
		reply += "\r\n";
	std::cout << "yoooo : " << reply << std::endl;
	return reply;
}

void	Server::sendHermes(std::string reply, std::vector<User *> recipients){
	if (recipients.empty())
		return ;
	for (int i = 0; i != recipients.size(); i++){
		std::cout << "daaamn : " << reply << std::endl;
		if (send(recipients[i]->sendFd, reply.c_str(), reply.length(), 0) == -1){
			this->lostConnection(recipients[i]);
			return;
		}
	}
	return ;
}