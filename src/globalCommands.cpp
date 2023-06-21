#include "Ft_irc.hpp"


void Server::handleOtherCmds(User *UserX, std::string* cmdParams, int paramNumber)
{
	if (DEBUG)
	{
		std::cout << "COMMAND = " << cmdParams[0] << std::endl;
		std::cout << "PARAMS = " << std::flush;
		printParams(cmdParams);
	}
	if (cmdParams[0] == "OPER" || cmdParams[0] == "oper"){
		this->handleCmdOper(cmdParams, UserX, paramNumber);
	}

	// example: if (cmdParams[0] == "PING" || cmdParams[0] == "ping")
		// this->handlePingCommand
}

