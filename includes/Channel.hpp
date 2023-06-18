#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include "User.hpp"

class User;
class   Channel{
    protected :
        std::string              channelKey;
        std::vector<User*>       channelOps;
        bool                     inviteFlag;
        bool                     keyAuth;
    public :
        std::string              channelTopic;
        std::vector<User*>      channelMembers;
        std::string              channelName;
        //int                      channelIndex;
    public :
        Channel();
        Channel(std::string s, std::string topic);
        Channel(std::string s, std::string topic, std::string pswd);
        Channel(Channel const &s);
        ~Channel();
        Channel const   &operator=(Channel const &s);
    public :
        // joinChannel(std::string name);
        // leaveChannel(std::string name);

};


#endif