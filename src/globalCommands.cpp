#include "Ft_irc.hpp"


void Server::handleOtherCmds(User *UserX, std::string* cmdParams, int paramNumber)
{
	if (DEBUG)
	{
		std::cout << "COMMAND = " << cmdParams[0] << std::endl;
		std::cout << "PARAMS = " << std::flush;
		printParams(cmdParams);
	}
	if (cmdParams[0] == "OPER" || cmdParams[0] == "oper")
		this->handleCmdOper(cmdParams, UserX, paramNumber);
	else if (cmdParams[0] == "JOIN" || cmdParams[0] == "join")
		this->ParseJoin(cmdParams , UserX, paramNumber);
	else if (cmdParams[0] == "MODE" || cmdParams[0] == "mode")
		this->handleCmdMode(cmdParams, UserX, paramNumber);
}

