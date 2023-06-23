#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include "User.hpp"

class User;
class   Channel{
    protected :
        size_t                          limit;
        std::string                     channelKey;
    public :
        std::vector<User *>             channelOps;
        std::vector<User*>              channelMembers;
        std::string                     channelName;
        std::string                     channelTopic;
        bool                            inviteMode;
        bool                            topicProtectMode;
        bool                            limitMode;
        bool                            keyMode;
    public :
        Channel();
        Channel(std::string s, std::string topic);
        Channel(std::string s, std::string topic, std::string pswd);
        Channel(Channel const &s);
        ~Channel();
        Channel const   &operator=(Channel const &s);
    public :
        int findUserinChannel(int fd);
        std::string GetThekey();
        void        setTheKey(std::string s);
        void        setTheLimit(size_t t);


};


#endif