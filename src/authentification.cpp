#include "../includes/Ft_irc.hpp"

bool	Server::authenticated(int fdClient)
{
	std::map<int, User *>::iterator it;
	for (it = this->users.begin(); it != this->users.end(); ++it)
	{
		if (it->first == fdClient)
			break;
	}
	if (it->second->userAuthentified == true)
		return (true);
	return (false);                                                                                                                                                                                                                                                                                                                                                                            
}

void    Server::firstConnection(int i, std::string buffer, User *UserX)
{
	int				size;
	int				paramNumber;
	std::string		*cmdParams;
	
	UserX->getCommands(buffer, true);
	size = UserX->commandFull.size();
	cmdParams = NULL;
	for (size_t j = 0; j < size; j++)
	{
	
		if (UserX->userAuthentified == FAILURE)
		{
			break;
		}
		if (!successiveSpace(UserX->commandFull[j]))	
		{

			paramNumber = countParams(UserX->commandFull[j]);
			cmdParams =  fillParams(UserX->commandFull[j], paramNumber);
			this->handleAuthentCmds(UserX, cmdParams, paramNumber - 1);
		}
	}
}

void	Server::sendStatusUpdate(int clientFd, User *userX, std::string com, std::string msg)
{
	std::string	message;

	if (com == "NICK")
		message = ":" + userX->oldNick + "!" + userX->getUserForReply() + "@" + userX->getHostForReply() 
	+ " " + com + " " + msg + "\r\n";
	else
		message = ":" +  userX->getNickForReply() + "!" + userX->getUserForReply() + "@" + userX->getHostForReply() 
		+ " " + com + " " + msg + "\r\n";
	correspondence(CLIENT_TO_SERVER, message);
	if (send(clientFd, message.c_str(), message.length(), 0) == -1)
		this->lostConnection(userX);
}

void Server::handleAuthentCmds(User *UserX, std::string* cmdParams, int paramNumber)
{
	bool	first;

	first = true;
	if (DEBUG)
	{
		std::cout << "NC = " << UserX->nc << std::endl;
		std::cout << "COMMAND = " << cmdParams[0] << std::endl;
		std::cout << "PARAMS = " << std::flush;
		printParams(cmdParams);
	}
	if (validCmd("pass", cmdParams[0]))
	{
		UserX->primer *= 3;
		this->handleCmdPass(cmdParams, UserX, paramNumber);
		
	}
	else if (validCmd("nick", cmdParams[0]))
	{
		if (first)
		{
			UserX->primer *= 5;
			first = true;
		}
		this->handleCmdNick(cmdParams, UserX, paramNumber);
	}
	else if (validCmd("user", cmdParams[0]))
	{
			UserX->primer *= 7;
			this->handleCmdUser(cmdParams, UserX, paramNumber);
	}
	delete[] cmdParams;
}

