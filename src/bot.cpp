#include "../includes/Ft_irc.hpp"

std::string	 botPing(User *userX, std::string nick, std::string cat)
{
	std::string	msg;
	std::string	all;
	if (validCmd("bath", cat))
		msg = "You have been pinged, you need a bath. Urgently!";
	else if (validCmd("feet", cat))
		msg = "You have been pinged, your feet stink!";
	else 
		msg = "You have been pinged, your armpit smell.";
	all = std::string(":anonymous") + " " + "NOTICE" + " " + nick + " :" + msg + "\r\n";
	return (all);
}

void     Server::bot(std::string	*params, User *userX ,int paramNumber)
{
	int			fd;
	std::string	msg;
	int			indx;
	if (userX->nc == true)
		indx = 0;
	else
		indx = 1;
	if (paramNumber == indx)
	{
		msg = std::string(GREEN) + "USMELL anonymously ping your peers who are kinda smelly\r\n" + std::string(RESET) + std::string(GREEN) + "categories: BATH FEET ARMPITS\r\n" + std::string(RESET) +  std::string(GREEN) + "USE: USMELL <categorie> <nick>" + std::string(RESET) + "\r\n";
		if (send(userX->sendFd, msg.c_str(), msg.length(), 0) == -1)
		{
			this->lostConnection(userX);
			return ;
		}
	}
	else if  (paramNumber != indx + 2)
	{
		msg = ":USMELL " + std::string(ERR_NORULES) + " " +  userX->getNick() + " :syntax error\r\n";
		if (send(userX->sendFd, msg.c_str(), msg.length(), 0) == -1)
		{
			this->lostConnection(userX);
			return ;
		}
	}
	else if (paramNumber == indx + 2)
	{
		fd =  this->findClient(params[indx + 2]);
		if (fd == -1)
		{
			msg = ":USMELL " + std::string(ERR_NORULES) + " " +  userX->getNick() + " :no such nick\r\n";
			if (send(userX->sendFd, msg.c_str(), msg.length(), 0) == -1)
			{
				this->lostConnection(userX);
				return ;
			}
		}
		else if (validCmd("bath", params[indx + 1]) || validCmd("feet", params[indx + 1]) || validCmd("armpits", params[indx + 1]) )
		{
			msg = botPing(userX, params[indx + 2],params[indx + 1]);
			if (send(fd, msg.c_str(), msg.length(), 0) == -1)
			{
				this->lostConnection(userX);
				return ;
			}
		}
		else
		{
			msg = msg = ":USMELL " + std::string(ERR_NORULES) + " " +  userX->getNick() + " : categories BATH FEET ARMPITS\r\n";
			if (send(userX->sendFd, msg.c_str(), msg.length(), 0) == -1)
			{
				this->lostConnection(userX);
				return ;
			}
		}
	}
}	

