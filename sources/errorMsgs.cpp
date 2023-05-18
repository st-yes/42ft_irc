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

/*--  errno variable --*/
void	errnoCheck(std::string	func)
{
	std::cerr << "ERROR ";
	std::cerr << func << ": " << strerror(errno);
	exit(EXIT_FAILURE);
}