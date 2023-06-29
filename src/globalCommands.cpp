#include "../includes/Ft_irc.hpp"


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
		this->handleCmdJoin(cmdParams , UserX, paramNumber);
	else if (cmdParams[0] == "MODE" || cmdParams[0] == "mode")
		this->handleCmdMode(cmdParams, UserX, paramNumber);
	else if (cmdParams[0] == "WHOIS")
		return;
	else if (cmdParams[0] == "PART" || cmdParams[0] == "part")
		this->handleCmdPart(cmdParams, UserX, paramNumber);
	else if (cmdParams[0] == "TOPIC" || cmdParams[0] == "topic")
		this->handleCmdTopic(cmdParams, UserX, paramNumber);
	else if (cmdParams[0] == "PING" || cmdParams[0] == "ping")
		this->handleCmdPing(cmdParams, UserX, paramNumber);
	else if (cmdParams[0] == "PING" || cmdParams[0] == "ping")
		this->handleCmdPing(cmdParams, UserX, paramNumber);
	else if (cmdParams[0] == "NICK"|| cmdParams[0] == "nick")
		this->handleCmdRegularNick(cmdParams, UserX, paramNumber);
	else if(cmdParams[0] == "PRIVMSG" || cmdParams[0] == "privmsg" )
		this->handleCmdMsg(cmdParams, UserX, paramNumber, PRIVMSG);
	 else if (cmdParams[0] == "NOTICE" || cmdParams[0] == "notice")
		this->handleCmdMsg(cmdParams, UserX, paramNumber, NOTICE);
	
}

