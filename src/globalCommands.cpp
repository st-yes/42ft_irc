#include "../includes/Ft_irc.hpp"

bool	validCmd(std::string	cmdLower, std::string input)
{
	std::string	newInput;
	for (int i = 0; i < input.length(); i++)
	{
		newInput += tolower(input[i]);
	}
	return (cmdLower == newInput);
}
void Server::handleOtherCmds(User *UserX, std::string* cmdParams, int paramNumber)
{
	if (DEBUG)
	{
		std::cout << "COMMAND = " << cmdParams[0] << std::endl;
		std::cout << "PARAMS = " << std::flush;
		printParams(cmdParams);
	}
	if (validCmd("oper", cmdParams[0]))
		this->handleCmdOper(cmdParams, UserX, paramNumber);
	else if (validCmd("join", cmdParams[0]))
		this->handleCmdJoin(cmdParams , UserX, paramNumber);
	else if (validCmd("mode", cmdParams[0]))
		this->handleCmdMode(cmdParams, UserX, paramNumber);
	else if (validCmd("part", cmdParams[0]))
		this->handleCmdPart(cmdParams, UserX, paramNumber);
	else if (validCmd("topic", cmdParams[0]))
		this->handleCmdTopic(cmdParams, UserX, paramNumber);
	else if (validCmd("ping", cmdParams[0]))
		this->handleCmdPing(cmdParams, UserX, paramNumber);
	else if (validCmd("nick", cmdParams[0]) )
		this->handleCmdRegularNick(cmdParams, UserX, paramNumber);
	else if(validCmd("privmsg", cmdParams[0]) )
	{
		if (UserX->nc == true && cmdParams[1].find(',') != std::string::npos)
			this->handleMultiMsg(cmdParams, UserX, paramNumber, PRIVMSG);
		else
			this->handleCmdMsg(cmdParams, UserX, paramNumber, PRIVMSG);
	}
	else if (validCmd("invite", cmdParams[0]))
		this->handleCmdInvite(cmdParams, UserX, paramNumber);
	else if (validCmd("kick", cmdParams[0]))
		this->handleCmdKick(cmdParams, UserX, paramNumber);
	else if (validCmd("notice", cmdParams[0]) )
	{
		if (UserX->nc == true && cmdParams[1].find(',') != std::string::npos)
			this->handleMultiMsg(cmdParams, UserX, paramNumber, NOTICE);
		else
			this->handleCmdMsg(cmdParams, UserX, paramNumber, NOTICE);
	}
	else if (validCmd("quit", cmdParams[0]))
		this->handleCmdQuit(cmdParams, UserX, paramNumber);
	else if ((UserX->nc == false && cmdParams[0] == "WHO" && cmdParams[1] == "USMELL") || (UserX->nc && validCmd("usmell", cmdParams[0])))
	{
		this->bot(cmdParams, UserX, paramNumber); //add nc version /usmell
	}
	delete [] cmdParams;
	return ;
}

// void Server::handleOtherCmds(User *UserX, std::string* cmdParams, int paramNumber)
// {
// 	if (DEBUG)
// 	{
// 		std::cout << "COMMAND = " << cmdParams[0] << std::endl;
// 		std::cout << "PARAMS = " << std::flush;
// 		printParams(cmdParams);
// 	}
// 	if (cmdParams[0] == "OPER" || cmdParams[0] == "oper")
// 		this->handleCmdOper(cmdParams, UserX, paramNumber);
// 	else if (cmdParams[0] == "JOIN" || cmdParams[0] == "join")
// 		this->handleCmdJoin(cmdParams , UserX, paramNumber);
// 	else if (cmdParams[0] == "MODE" || cmdParams[0] == "mode")
// 		this->handleCmdMode(cmdParams, UserX, paramNumber);
// 	else if (cmdParams[0] == "WHOIS")
// 		return;
// 	else if (cmdParams[0] == "PART" || cmdParams[0] == "part")
// 		this->handleCmdPart(cmdParams, UserX, paramNumber);
// 	else if (cmdParams[0] == "TOPIC" || cmdParams[0] == "topic")
// 		this->handleCmdTopic(cmdParams, UserX, paramNumber);
// 	else if (cmdParams[0] == "PING" || cmdParams[0] == "ping")
// 		this->handleCmdPing(cmdParams, UserX, paramNumber);
// 	else if (cmdParams[0] == "NICK"|| cmdParams[0] == "nick")
// 		this->handleCmdRegularNick(cmdParams, UserX, paramNumber);
// 	else if(cmdParams[0] == "PRIVMSG" || cmdParams[0] == "privmsg" )
// 		this->handleCmdMsg(cmdParams, UserX, paramNumber, PRIVMSG);
// 	else if (cmdParams[0] == "INVITE" || cmdParams[0] == "invite")
// 		this->handleCmdInvite(cmdParams, UserX, paramNumber);
// 	else if (cmdParams[0] == "KICK" || cmdParams[0] == "kick")
// 		this->handleCmdKick(cmdParams, UserX, paramNumber);
// 	else if (cmdParams[0] == "NOTICE" || cmdParams[0] == "notice")
// 		this->handleCmdMsg(cmdParams, UserX, paramNumber, NOTICE);
// 	else if (cmdParams[0] == "QUIT" || cmdParams[0] == "quit")
// 		this->handleCmdQuit(cmdParams, UserX, paramNumber);
// 	else if (UserX->nc == false && cmdParams[0] == "WHO" && cmdParams[1] == "USMELL")
// 	{
// 		this->bot(cmdParams, UserX, paramNumber); //add nc version /usmell
// 	}
// 	return ;
// }


