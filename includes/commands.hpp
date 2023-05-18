#pragma once

# define MAX_PORT 65535
# define MIN_PORT 1
# define BACKLOG 10
# define BUFFER_SIZE 500


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
void	errnoCheck(std::string	func);
