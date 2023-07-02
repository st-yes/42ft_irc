#include "../includes/Ft_irc.hpp"

/*----CMDS: PASS, NICK, USER */

/*
* if PASS not given OR incorrect: ERR_PASSWDMISMATCH
* will i use ERR_NEEDMOREPARAMS ?
*/
void	Server::handleCmdPass(std::string	*params, User *userX ,int paramNumber)
{
	std::string	*paramsRep;
	std::map<int, User*>::iterator it;
	std::vector<User*>			   send;
	
	send.push_back(userX);
	if (userX->primer != 3)
	{
		// paramsRep = allocateForParams(2);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[1] = ":Invalid authentication order";
		// sendReply(userX->sendFd, ERR_NORULES, paramsRep);
		// delete[] paramsRep;
		// this->fatalError(clientFd, "PASS not set");
		// close(clientFd);
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "Invalid authentication order!"), send);
	}
	else if (paramNumber < 1)
	{
		// paramsRep = allocateForParams(3);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[0] = "PASS";
		// paramsRep[1] = ":Not enough parameters";
		// sendReply(userX->sendFd, ERR_NEEDMOREPARAMS, paramsRep);
		// delete[] paramsRep;
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "need more parameters"), send);
	}
	else if (paramNumber > 1 || !this->passCorrect(params[1]))
	{
		// paramsRep = allocateForParams(2);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[1] = ":Password incorrect";
		// sendReply(userX->sendFd, ERR_PASSWDMISMATCH, paramsRep);
		// delete[] paramsRep;
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_PASSWDMISMATCH, "Password incorrect"), send);
	}
	else if (userX->passSet)
	{
		// paramsRep = allocateForParams(2);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[1] = ":You may not register";
		// sendReply(userX->sendFd, ERR_ALREADYREGISTERED, paramsRep);
		// delete[] paramsRep;
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_ALREADYREGISTERED, "already registered you may not do it again"), send);
	}
	else
		userX->passSet = true;
}


void	Server::handleCmdNick(std::string	*params, User *userX ,int paramNumber)
{
	//std::cout << "farakiss" << std::endl;
	std::string	*paramsRep;
	//std::cout << "hali :: -------->" << userX->primer << std::endl;
	std::vector<User *>	send;
	send.push_back(userX);
	if (userX->primer == 5)
	{
		// paramsRep = allocateForParams(2);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[1] = ":Invalid authentication order";
		// sendReply(userX->sendFd, ERR_NORULES, paramsRep);
		// delete[] paramsRep;
		// this->fatalError(clientFd, "PASS not set");
		// close(clientFd);
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "invalid authentication order"), send);
	}
	else if (!userX->passSet)
	{
		// std::cout << "wa 3ok3ok" << std::endl;
		// paramsRep = allocateForParams(2);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[1] = ":Pass not set";
		// sendReply(userX->sendFd, ERR_NORULES, paramsRep);
		// delete[] paramsRep;
		// this->fatalError(clientFd, "PASS not set");
		// close(clientFd);
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "PASS not set"), send);
	}
	else if (paramNumber < 1)
	{
		// paramsRep = allocateForParams(2);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[1] = ":No nickname given";
		// sendReply(userX->sendFd, ERR_ERRONEUSNICKNAME, paramsRep);
		// delete[] paramsRep;
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
		std::cout << "waaaaaa rbi" << std::endl;
		if (nickAlreadyInUse(params[1]))
		{
			// std::cout << 5 << std::endl;
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
			if (userX->userAuthentified == NICK_AGAIN) // no touch
			{
				std::cout << 6 << std::endl;
				userX->setNick(params[1]);
				this->sendStatusUpdate(userX->sendFd, userX, "NICK", params[1]);
				userX->userAuthentified = false;
				//userX->oldNick = userX->userNick;
				//std::cout << "primer = " << userX->primer << std::endl;
				if (userX->primer == 3 * 5 * 7 && userX->userAuthentified == false && userX->passSet == true)
				{
					std ::cout << "waaaaa" << std::endl;
					userX->userAuthentified = true;
					//this->sendWelcome(userX);
					this->sendHermes(this->sendNumericCode(userX, NULL, RPL_WELCOME, ""), send);
				}
			}
			userX->setNick(params[1]);
			if (userX->primer == 3 * 5 * 7 && userX->userAuthentified  == false && userX->passSet == true)
			{
				std::cout << 7 << std::endl;
				std::cout << "bananaaaaa " << std::endl;
				userX->userAuthentified = true;
				//this->sendWelcome(userX);
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
		// paramsRep = allocateForParams(2);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[1] = ":Invalid authentication order";
		// sendReply(userX->sendFd, ERR_NORULES, paramsRep);
		// delete[] paramsRep;
		// this->fatalError(clientFd, "PASS not set");
		// close(clientFd);
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "pass was not set"), send);
	}
	else if (!userX->passSet)
	{
		// paramsRep = allocateForParams(2);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[1] = ":Pass not set";
		// sendReply(userX->sendFd, ERR_NORULES, paramsRep);
		// delete[] paramsRep;
		// this->fatalError(clientFd, "PASS not set");
		// close(clientFd);
		userX->userAuthentified = FAILURE;
		this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NORULES, "pass was not set"), send);
	}
	else if (paramNumber != 4)
	{
		// paramsRep = allocateForParams(3);
		// paramsRep[0] = userX->getNickForReply();
		// paramsRep[1] = "USER";
		// paramsRep[2] = ":Invalid number of parameters";
		// sendReply(userX->sendFd, ERR_NORULES, paramsRep);
		// delete[] paramsRep;
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
			//this->sendWelcome(userX);
			this->sendHermes(this->sendNumericCode(userX, NULL, RPL_WELCOME, ""), send);
		}
	}
}