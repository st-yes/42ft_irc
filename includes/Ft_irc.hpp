#pragma once 

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "ircGod.hpp"
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

enum flags
{
	SERVER_TO_CLIENT = 2,
	CLIENT_TO_SERVER,
	FAILURE,
	NICK_AGAIN,
	NO_RESET
};

void	errorMsg(int error);
bool	validPort(std::string s);
bool	validPass(std::string s);
void	errnoCheck(std::string	func);
bool    parse_args(char **s);
void	parsePassNick(User *newUser, std::string buffer, int start, int comm);
bool	parseUser(User *newUser, std::string buffer, int start);
void	getCredentials(User *newUser, std::string buffer, int start, int comm);
bool    checkAttr(std::string s);
int     checkStringToken(std::string *params, int paramNumber);
size_t	checkNums(std::string s);

/*--- PARSING UTILS ---*/
int				countParams(std::string	command);
void 			tokenizeCmd(std::string& cmd, std::string *params);
std::string		*fillParams(std::string cmd, int size);
bool			isAllSpace(std::string	str);
std::string		*allocateForParams(int size);
std::string		assembleParmsIntoToken(std::string * params);
std::string *getCmdParams(char *buffer, User *userX, int *paramNumber);

/*---- DEBUG ----*/
void	printParams(std::string *params);
void	trimLast(std::string&	str);
bool 	successiveSpace(std::string	cmd);
void	correspondence(int way, std::string buff);