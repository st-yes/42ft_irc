#include "../includes/Channel.hpp"

Channel::Channel(){}
Channel::Channel(std::string s, std::string topic) : channelName(s), channelTopic(topic){
    std::cout << this->channelName + " has been created!" << std::endl;
}
Channel::Channel(std::string s, std::string topic, std::string pswd) : channelName(s), channelTopic(topic), keyMode(true) ,channelKey(pswd){
    std::cout << this->channelName + " has been created!" << std::endl;
}
Channel::Channel(Channel const &s){
    this->channelName = s.channelName;
    this->channelTopic = s.channelTopic;
    for (int i = 0; i < s.channelOps.size(); i++){
        this->channelOps.push_back(s.channelOps[i]);
    }
    for (int j = 0; j < s.channelMembers.size(); j++){
        this->channelMembers.push_back(s.channelMembers[j]);
    }
}
Channel::~Channel(){
    std::cout << "The Channel : " + channelName + " is unusable anymore!" << std::endl;
}
Channel const &Channel::operator=(Channel const &s){
    *this = s;
    return *this;
}
int    Channel::findUserinChannel(int fd){
    if(this->channelMembers.empty())
    {
        return -1;}
    for(std::vector<User*>::iterator it = this->channelMembers.begin() ; it != this->channelMembers.end(); it++){

        if (*it && (*it)->sendFd == fd){
            std::cout << "found " << std::endl;
            return (it - this->channelMembers.begin());
        }
    }
    return -1;
}
void    Channel::setTheKey(std::string s){
    if (this->keyMode)
    {
        this->channelKey = s;
        return;
    }
        return;
}
void    Channel::setTheLimit(size_t lim){
    if (this->limitMode){
        this->limit = lim;
        return;
    }
    return;
}