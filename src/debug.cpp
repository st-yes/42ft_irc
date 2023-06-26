#include "../includes/Ft_irc.hpp"

void	printParams(std::string *params)
{
	int i;

	i = 1;
	while (params[i] != "")
		std::cout << params[i++] << " | " << std::flush;
	if (i == 1)
		std::cout << "[ 0 parameters]" <<  std::flush;
	std::cout << std::endl;
}
void	correspondence(int way, std::string buff)
{
	if (way == SERVER_TO_CLIENT)
		std::cout << "[server->client]" << buff << std::flush;
	else if (way == CLIENT_TO_SERVER)
		std::cout << "[client->server]" << buff << std::flush;
}