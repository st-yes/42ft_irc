#include "User.hpp"

    User::User(){}
    User::User(std::string name, std::string nick, std::string fullName, std::string hostName) : userName(name), userNick(nick), userFullName(fullName), userHostName(hostName){
        this->currentChannel = NULL;
        this->nextChannel = NULL;
        //std::cout << this->userNick + " has been created!" << std::endl;
    }
    User::User(User const &s){
        this->sendFd = s.sendFd;
        this->primer = s.primer;
        this->userNick = s.userNick;
        this->userName = s.userName;
        this->userHostName = s.userHostName;
        this->userFullName = s.userFullName;
        this->serverName = s.serverName;
        this->userAuthentified = s.userAuthentified;
        this->userPass = s.userPass;
        this->currentChannel = s.currentChannel;
        this->nextChannel = s.nextChannel;
    }
    User::~User(){
        //std::cout << this->userNick + " has been deleted!" << std::endl;
    }
    User const &User::operator=(User const &s){
        *this = s;
        return *this;
    }
/*-------------------------------------------------------Setters-&-Getters-----------------------------------------------------*/
    void    User::setNick(std::string const s){
        std::cout << "what is happening here :::::: " << s << std::endl;
        this->userNick = s;
    }
    void    User::setUsrName(std::string const s){
        this->userName = s;
    }
    void    User::setFullName(std::string const s){
        this->userFullName = s;
    }
    void    User::setUsrHostName(std::string const s){
        this->userHostName = s;
    }
    void    User::setUsrName(std::string const s, int mode){
        this->userName += s;
    }
    void    User::setFullName(std::string const s, int mode){
        this->userFullName += s;
    }
    void    User::setUsrHostName(std::string const s, int mode){
        this->userHostName += s;
    }
    std::string const User::getNick(){
        return (this->userNick);
    }
    std::string const User::getUsrName(){
        return (this->userName);
    }
    std::string const User::getFullName(){
        return (this->userFullName);
    }
    std::string const User::getUsrHostName(){
        return (this->userHostName);
    }

bool    User::validNick(std::string nick)
{
    if (nick[0] == '#' || nick[0] == '"')
        return (false);
    return (true);
}

std::string	User::getNickForReply()
{
	if (this->userNick.empty())
		return ("*");
	else
		return (this->userNick);
}
std::string	User::getUserForReply()
{
	if (this->userName.empty())
		return ("*");
	else
		return (this->userName);
}
std::string	User::getHostForReply()
{
	if (this->userHostName.empty())
		return ("*");
	else
		return (this->userHostName);
}