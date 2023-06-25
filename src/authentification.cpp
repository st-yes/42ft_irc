#include "Ft_irc.hpp"

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

void    Server::firstConnection(int i, char *buffer, User *UserX)
{
	int				size;
	int				paramNumber;
	std::string		*cmdParams;
	
	std::string buff(buffer);
	UserX->getCommands(buff, true);
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
	if (send(clientFd, message.c_str(), message.length(), 0) == -1)
		throw errorErrno(); // check for error
}
/* first: PASS, NICK, USER 
* function to generate the alternatives : /PASS /pass PASS pass 
*/
void Server::handleAuthentCmds(User *UserX, std::string* cmdParams, int paramNumber)
{
	bool	first;

	first = true;
	if (DEBUG)
	{
		std::cout << "COMMAND = " << cmdParams[0] << std::endl;
		std::cout << "PARAMS = " << std::flush;
		printParams(cmdParams);
	}
	if (cmdParams[0] == "PASS" || cmdParams[0] == "pass")
	{
		UserX->primer *= 3;
		this->handleCmdPass(cmdParams, UserX, paramNumber);
		
	}
	else if (cmdParams[0] == "NICK" || cmdParams[0] == "nick")
	{
		if (first)
		{
			UserX->primer *= 5;
			first = true;
		}
		this->handleCmdNick(cmdParams, UserX, paramNumber);
		std::cout << "waaaa l3aadaaaaw" << UserX->userAuthentified << std::endl;
	}
	else if (cmdParams[0] == "USER" || cmdParams[0]== "user")
	{
			UserX->primer *= 7;
			this->handleCmdUser(cmdParams, UserX, paramNumber);
	}
}

