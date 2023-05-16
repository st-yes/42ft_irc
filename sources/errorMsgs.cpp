# include "Server.hpp"

void	errorMsg(int error)
{
	std::cerr << "ERROR: ";
	if (error == USAGE)
		std::cerr << "usage: ./banana PORT PASSWORD" << std::endl;
	else if (error == PASSWORD)
		std::cerr << "invalid password" << std::endl;
	else if (error == PORT)
		std::cerr << "invalid port number" << std::endl;
}

void	errorExit(int error)
{
	std::cerr << "ERROR: ";
	if (error == SOCKET_FD)
		std::cerr << "server unavailable " << std::endl;
	exit(EXIT_FAILURE);
}