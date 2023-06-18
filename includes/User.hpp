#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <sstream>
# include "Channel.hpp"

class Channel;
class User{
    protected :
    public :
        int         sendFd;
        int         primer;
        std::string userName;
        std::string userFullName;
        std::string userHostName;
        std::string userNick;
        std::string serverName;
        std::string channelName;
        std::string userCommand;
        std::string userPass;
        bool        welcomeMessage;
        bool        passAuth;
        bool        userAuthentified;
        bool        chanOpPermission;
        Channel     *currentChannel;
    public :
        User();
        User(std::string name, std::string nick, std::string userFullName, std::string userHostName);
        User(User const &s);
        ~User();
        User const  &operator=(User const & s);
    /*-------------------Setters------------------*/
    public :
        void    setNick(std::string const s);
        void    setNick(std::string const s, int mode);
        void    setUsrName(std::string const s);
        void    setUsrName(std::string const s, int mode);
        void    setFullName(std::string const s);
        void    setFullName(std::string const s, int mode);
        void    setUsrHostName(std::string const s);
        void    setUsrHostName(std::string const s, int mode);
    /*-------------------Getters------------------*/
    public :
        std::string const getNick();
        std::string const getUsrName();
        std::string const getFullName();
        std::string const getUsrHostName();
};

#endif