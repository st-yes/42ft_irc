#pragma once 

#include "Server.hpp"
#include "User.hpp"
#include "Request.hpp"
#include <iostream>

# define MAX_PORT 65535
# define MIN_PORT 1
# define BACKLOG 10
# define BUFFER_SIZE 512

# define RPL_WELCOME "001"
# define ERR_NEEDMOREPARAMS "461"
# define ERR_PASSWDMISMATCH "464"
# define ERR_NICKNAMEINUSE "433"
# define ERR_NICKCOLLISION "436"

enum	errors
{
	USAGE,
	PASSWORD,
	PORT,
	SOCKET_FD
};

enum	commands
{
	PASS,
	NICK,
	USER
};

void	errorMsg(int error);
bool	validPort(std::string s);
bool	validPass(std::string s);
void	errnoCheck(std::string	func);
bool    parse_args(char **s);
void	parsePassNick(User *newUser, std::string buffer, int start, int comm);
bool	parseUser(User *newUser, std::string buffer, int start);
void	getCredentials(User *newUser, std::string buffer, int start, int comm);