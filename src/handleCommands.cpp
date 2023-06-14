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
	if (send(clientFd, reply.c_str(), reply.length(), 0) == -1)
		throw errorErrno();
}

/* connection established */
void	Server::sendWelcome(int	clientFd, User *user)
{
	std::string	msg;

	msg = ":" + user->serverName + " " + RPL_WELCOME + " " + user->getNick() + " :Welcome to BANANA TASBA7 " + user->getNick() + "!" + user->getUsrName() + "@" + user->getUsrHostName() + "\r\n";
	if (send(clientFd, msg.c_str(), msg.length(), 0) == -1)
		throw errorErrno();
    msg = ":" + user->getNick() + " has joined the server!\r\n"; //message send to all those connected to socket ??
    for (int i = 0; i < this->pollers.size(); i++)
	{
        if (this->pollers[i].fd == this->servSocketFd)
            continue;
		if (authenticated(this->pollers[i].fd))
		{
			if (send(this->pollers[i].fd, msg.c_str(), msg.length(), 0) == -1)
				throw errorErrno();
		}
    }
}

/* even without "\r\n" it get sent to official client */
void	Server::sendInstructions(int clientFd)
{
	std::string	instruct;
	instruct = "You are trying to connect from a non official irc client\nPlease provide:\nP A S S\nN I C K\nU S E R\r\n";
	if (send(clientFd, instruct.c_str(), instruct.length(), 0) == -1)
		throw errorErrno();
}

void Server::fetchTheFirst(std::string command, std::string buffer, User *newUser)
{
	std::string commandUp;
	int			i;

	for (int i = 0; i < command.length(); i++)
		commandUp += std::toupper(command[i]);
	if (buffer.find(command) != std::string::npos || buffer.find(commandUp) != std::string::npos)
	{
		if (buffer.find(command) != std::string::npos)
			i = buffer.find(command);
		else
			i = buffer.find(commandUp);
		i += 4;
		if (command == "pass")
			getCredentials(newUser, buffer, i, PASS);
		else if (command == "nick")
			getCredentials(newUser, buffer, i, NICK);
		else if (command == "user")
			getCredentials(newUser, buffer, i, USER);
	}
}

/* search for the commands: PASS NICK USER; upper or lower case*/
int Server::searchForCredentials(std::string buffer, User *newUser)
{

	static int 		mult = 1;
	int				i;

	if (buffer.find("pass") != std::string::npos 
	|| buffer.find("PASS") != std::string::npos)
	{
		fetchTheFirst("pass", buffer, newUser);
		mult *= 3;
	}
	if (buffer.find("nick") != std::string::npos
	|| buffer.find("NICK") != std::string::npos)
	{
		fetchTheFirst("nick", buffer, newUser);
		mult *= 5;
	}
	if (buffer.find("user") != std::string::npos
	|| buffer.find("USER") != std::string::npos)
	{
		fetchTheFirst("user", buffer, newUser);
		mult *= 7;
	}
	return (mult);
}