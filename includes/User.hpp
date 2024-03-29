#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <sstream>
# include <map>
# include "Channel.hpp"

class Channel;
class User{
    protected :
        std::string userName;
        std::string userNick;
        std::string userHostName;
        std::string userFullName;
        std::string userPass;
    public :
        int                         userAuthentified;
        int                         sendFd;
        int                         primer;
        Channel                     *defaultChannel;
        std::string                 serverName;
        std::string                 oldNick;
        std::vector<Channel *>      invitedChannels;
        std::vector<Channel *>      joinedChannels;
        std::vector<std::string>    commandFull;
        bool                        passSet;
        bool                        nickSet;
        bool                        nc;
        int                         createdChannels;
    public :
        User();
        User(std::string name, std::string nick, std::string userFullName, std::string userHostName);
        User(User const &s);
        virtual ~User();
        User const  &operator=(User const & s);

    /*-----------------------------Setters------------------------------------*/
    public :
        void    setNick(std::string const s);
        void    setUsrName(std::string const s);
        void    setUsrName(std::string const s, int mode);
        void    setFullName(std::string const s);
        void    setFullName(std::string const s, int mode);
        void    setUsrHostName(std::string const s);
        void    setUsrHostName(std::string const s, int mode);
    /*-----------------------------Getters------------------------------------*/
    public :
        std::string const getNick();
        std::string const getUsrName();
        std::string const getFullName();
        std::string const getUsrHostName();
        std::string         getNickForReply();
        std::string         getUserForReply();
        std::string         getHostForReply();
    /*-------------------------**Member-Functions----------------------------*/
    public :
        void    getCommands(std::string	buffer, bool reset);
        bool    validNick(std::string s);
        void    setNc(std::string buffer);
        Channel *channelFinderinUser(std::string s);
};

#endif