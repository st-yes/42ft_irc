#include "Ft_irc.hpp"


bool	Server::passCorrect(std::string passUser)
{
	if (passUser == this->passWord)
		return (true);
	return (false);
}

bool	Server::nickAlreadyInUse(std::string nick)
{
	std::map<int, User *>::iterator it;

	for (it = this->users.begin(); it != this->users.end(); ++it)
	{
		if (it->second->getNick() == nick)
			return (true);
	}
	return (false);
}

int	Server::findUserinChan(int fd, std::vector<User *> x){
	if (x.empty())
		return -1;
	for(std::vector<User *>::iterator it = x.begin(); it != x.end(); it++){
		if ((*it)->sendFd == fd)
			return it - x.begin();
	}
	return -1;
}

User *Server::findUserinServ(std::string s){
	if (this->users.empty())
		return NULL;
	for(std::map<int, User *>::iterator it = this->users.begin(); it != this->users.end(); it++){
		if (it->second->getNick() == s)
			return (it->second);
	}
	return NULL;
}

void	Server::deleteFromPoll(int fd){
	if (this->pollers.empty())
		return;
	for(std::vector<pollfd>::iterator it = this->pollers.begin(); it != this->pollers.end(); it++){
		if (it->fd == fd){
			this->pollers.erase(it);
			return ;
		}
	}
	return;
}

int     checkStringToken(std::string *params, int paramNumber){
    int val = 0;
    int val2 = 0;
    int place = 100;
    for (int i = 1; i <= paramNumber; i++){
        for (int k = 0; k != params[i].length(); k++){
            if (params[i].c_str()[k] == '#')
                val++;
            else if (params[i].c_str()[k] == '+' || params[i].c_str()[k] == '-'){
                val2++;
                place = i;
            }
        }
    }
    if (val > 1 || val2 > 1 || val2 == 0 || place > 2)
        return 4;
    else if (checkAttr(params[place]) && val == 1)
        return place;
    return 4;
}

bool    checkAttr(std::string s){
	if(!s.length())
		return false;
    for (int i = 1; i < s.length(); i++){
		std::cout << "|" << s.c_str()[i] << "|" << std::endl;
        if (s.c_str()[i] == 't' || s.c_str()[i] == 'l' || s.c_str()[i] == 'i' || s.c_str()[i] == 'k' || s.c_str()[i] == 'o')
            return true;
    }
    return false;
}

size_t	checkNums(std::string s){
	if(!s.length())
		return false;
	for (int i = 0; i < s.length(); i++){
		if (s.c_str()[i] < '1' && s.c_str()[i] > '9')
			return false;
	}
	return true;
}

// void	alternatives(std::string command, std::string input)
// {
// 	std::string	commandUP = // all uppercase
// 	std::string	commandlower = //all lowecase 
// }