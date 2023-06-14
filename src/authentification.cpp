#include "Ft_irc.hpp"

bool	Server::authenticated(int fdClient)
{
	std::map<int, User *>::iterator it;
	for (it = this->users.begin(); it != this->users.end(); ++it)
	{
		if (it->first == fdClient)
			break;
	}
	if (it->second->userAuthentified == true)
		return (true);
	return (false);                                                                                                                                                                                                                                                                                                                                                                            
}