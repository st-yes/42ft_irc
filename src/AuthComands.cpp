#include "../includes/Ft_irc.hpp"

//need to add lost connection

void	Server::handleCmdPass(std::string	*params, User *userX ,int paramNumber)
{
	std::string	*paramsRep;
	std::map<int, User*>::iterator it;
	std::vector<User*>			   send;
	
	send.push_back(userX);
	if (userX->primer != 3)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "Invalid authentication order!"), send);
	}
	else if (paramNumber < 1)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "need more parameters"), send);
	}
	else if (paramNumber > 1 || !this->passCorrect(params[1]))
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_PASSWDMISMATCH, "Password incorrect"), send);
	}
	else if (userX->passSet)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_ALREADYREGISTERED, "already registered you may not do it again"), send);
	}
	else
		userX->passSet = true;
}


void	Server::handleCmdNick(std::string	*params, User *userX ,int paramNumber)
{
	std::string	*paramsRep;
	std::vector<User *>	send;
	send.push_back(userX);
	if (userX->primer == 5)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "invalid authentication order"), send);
	}
	else if (!userX->passSet)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "PASS not set"), send);
	}
	else if (paramNumber < 1)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_ERRONEUSNICKNAME, "no nickname given"), send);
	}
	else if (paramNumber > 1 || !userX->validNick(params[1]))
	{
		paramsRep = allocateForParams(3);
		paramsRep[0] = userX->getNickForReply();
		paramsRep[1] = assembleParmsIntoToken(&params[1]);
		paramsRep[2] = ":invalid nickname";
		sendReply(userX->sendFd, ERR_ERRONEUSNICKNAME, paramsRep);
		delete[] paramsRep;
		userX->userAuthentified = FAILURE;
	}
	else
	{
		if (nickAlreadyInUse(params[1]))
		{
			userX->oldNick = params[1];
			paramsRep = allocateForParams(3);
			paramsRep[0] = userX->getNickForReply();
			paramsRep[1] = params[1];
			paramsRep[2] = ":Nickname is already in use";
			sendReply(userX->sendFd, ERR_NICKNAMEINUSE, paramsRep);
			delete[] paramsRep;
			userX->userAuthentified = NICK_AGAIN;
		}
		else
		{
			if (userX->userAuthentified == NICK_AGAIN)
			{
				userX->setNick(params[1]);
				this->sendStatusUpdate(userX->sendFd, userX, "NICK", params[1]);
				userX->userAuthentified = false;
				if (userX->primer == 3 * 5 * 7 && userX->userAuthentified == false && userX->passSet == true)
				{
					userX->userAuthentified = true;
					this->sendHermes(this->sendNumericCode(userX, NULL, RPL_WELCOME, ""), send);
				}
			}
			userX->setNick(params[1]);
			if (userX->primer == 3 * 5 * 7 && userX->userAuthentified  == false && userX->passSet == true)
			{
				userX->userAuthentified = true;
				 this->sendHermes(this->sendNumericCode(userX, NULL, RPL_WELCOME, ""), send);
			}
		}
	}
}

void	Server::handleCmdUser(std::string	*params, User *userX ,int paramNumber)
{
	std::string	*paramsRep;
	std::vector<User *>	send;
	send.push_back(userX);

	if (userX->primer == 7)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "pass was not set"), send);
	}
	else if (!userX->passSet)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "pass was not set"), send);
	}
	else if (paramNumber != 4)
	{
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "invalid number of parameters"), send);
	}
	else if (paramNumber == 4)
	{
		userX->setUsrName(params[1]);
		userX->setUsrHostName(params[2]);
		userX->serverName = params[3];
		userX->setFullName(params[4]);
		if (userX->primer == 3 * 5 * 7 && userX->userAuthentified == false && userX->passSet == true)
		{
			userX->userAuthentified = true;
			this->sendHermes(this->sendNumericCode(userX, NULL, RPL_WELCOME, ""), send);
		}
	}
}