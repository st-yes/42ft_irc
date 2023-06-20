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
	int		size;
	int		paramNumber;
	std::string	*cmdParams;
	std::string buff(buffer);

	UserX->getCommands(buff);
	size = UserX->commandFull.size();
	cmdParams = NULL;
	for (size_t j = 0; j < size; j++)
	{
		if (!successiveSpace(UserX->commandFull[j]))	
		{
			paramNumber = countParams(UserX->commandFull[j]);
			cmdParams =  fillParams(UserX->commandFull[j], paramNumber);
			this->handleAuthentCmds(UserX, cmdParams, paramNumber - 1);
		}
	}
}

/* first: PASS, NICK, USER 
* function to generate the alternatives : /PASS /pass PASS pass 
*/
void Server::handleAuthentCmds(User *UserX, std::string* cmdParams, int paramNumber)
{
	if (cmdParams[0] == "PASS" || cmdParams[0] == "pass")
	{
		//std::cout << "here??" << std::endl; 
		UserX->primer *= 3;
		this->handleCmdPass(cmdParams, UserX, paramNumber);
		
	}
	else if (cmdParams[0] == "NICK" || cmdParams[0] == "nick")
	{
			UserX->primer *= 5;
			this->handleCmdNick(cmdParams, UserX, paramNumber);
			
			//this->handleCmdNick(params, UserX, paramNumber);
	}
	else if (cmdParams[0] == "USER" || cmdParams[0]== "user")
	{
			UserX->primer *= 7;
			this->handleCmdUser(cmdParams, UserX, paramNumber);
			
			//this->handleCmdUser(params, UserX, paramNumber);
	}
}

