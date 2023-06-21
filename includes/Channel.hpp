#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include "User.hpp"

class User;
class   Channel{
    protected :
    public :
        std::vector<User*>       channelMembers;
        std::vector<User*>       channelOps;
        std::string              channelTopic;
        std::string              channelName;
        std::string              channelKey;
        bool                     inviteMode;
        bool                     limitMode;
        bool                     keyMode;
        size_t                   limit;
        //int                      channelIndex;
    public :
        Channel();
        Channel(std::string s, std::string topic);
        Channel(std::string s, std::string topic, std::string pswd);
        Channel(Channel const &s);
        ~Channel();
        Channel const   &operator=(Channel const &s);
    public :
        int findUserinChannel(int fd);
        // joinChannel(std::string name);
        // leaveChannel(std::string name);

};


#endif