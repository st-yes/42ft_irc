#include "Ft_irc.hpp"

void Server::handleCmdQuit(std::string	*params, User *userX ,int paramNumber, int flag)
{
	generateMsgPriv(userX, params, paramNumber, QUIT);
}