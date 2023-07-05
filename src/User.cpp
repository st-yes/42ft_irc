#include "../includes/User.hpp"

    User::User(){}
    User::User(std::string name, std::string nick, std::string fullName, std::string hostName) : userName(name), userNick(nick), userFullName(fullName), userHostName(hostName){

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
        this->passSet = s.passSet;
        this->nickSet = s.nickSet;
        this->nc      = s.nc;
        this->oldNick = s.oldNick;
        this->defaultChannel = s.defaultChannel;
        this->createdChannels = s.createdChannels;
        for(int i = 0; i != s.invitedChannels.size(); i++)
            this->invitedChannels.push_back(s.invitedChannels[i]);
        for(int i = 0; i != s.joinedChannels.size(); i++)
            this->joinedChannels.push_back(s.joinedChannels[i]);
        for(int i = 0; i != s.commandFull.size(); i++)
            this->commandFull.push_back(s.commandFull[i]);
    }
    User::~User(){
        if (!this->joinedChannels.empty())
            this->joinedChannels.clear();
        if (!this->invitedChannels.empty())
            this->invitedChannels.clear();
        if (!this->commandFull.empty())
            this->commandFull.clear();
        std::cout << "User has been destroyed" << std::endl;
    }
    User const &User::operator=(User const &s){
        *this = s;
        return *this;
    }
/*-------------------------------------------------------Setters-&-Getters-----------------------------------------------------*/
    void    User::setNick(std::string const s){
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
    if (nick[0] == '#' || nick[0] == '"' || nick.find(",") != std::string::npos || nick == "USMELL" || nick == "usmell")
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

void    User::setNc(std::string buffer)
{
    std::size_t pos = buffer.find("\r\n");
    if (pos == std::string::npos)
        this->nc = true;
}