#include "Ft_irc.hpp"

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
