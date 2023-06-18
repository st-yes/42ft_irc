#include "../includes/Channel.hpp"

Channel::Channel(){}
Channel::Channel(std::string s, std::string topic) : channelName(s), channelTopic(topic){
    std::cout << this->channelName + " has been created!" << std::endl;
}
Channel::Channel(std::string s, std::string topic, std::string pswd) : channelName(s), channelTopic(topic), keyAuth(true) ,channelKey(pswd){
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