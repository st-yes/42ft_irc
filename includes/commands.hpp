#pragma once

# define MAX_PORT 65535
# define MIN_PORT 1

enum	errors
{
	USAGE,
	PASSWORD,
	PORT,
	SOCKET_FD
};

void	errorMsg(int error);
bool	validPort(std::string s);
bool	validPass(std::string s);
void	errorExit(int error);