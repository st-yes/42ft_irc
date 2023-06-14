#include "Ft_irc.hpp"

/*-- check if port number is valid --*/
bool	validPort(std::string s)
{
	int	len;

	len = s.size();
	for (int i = 0; i < len; i++)
		if (!std::isdigit(s[i]))
			return (false);
	if (std::atoi(s.c_str()) < MIN_PORT || std::atoi(s.c_str()) > MAX_PORT)
		return (false);
	return (true);
}

/*-- check if the password is valid --*/
bool	validPass(std::string s)
{
	int	len;

	len = s.size();
	for (int i = 0; i < len; i++)
		if (isspace(s[i]))
			return (false);
	return (true);
}

