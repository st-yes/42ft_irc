#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <sstream>
# include <map>
# include "Channel.hpp"

# define MAX_PORT 65535
# define MIN_PORT 1
# define BACKLOG 10
# define BUFFER_SIZE 512
# define DEBUG 1

# define SERVERNAME "BANANA"

/*--- numeric reply --*/
# define RPL_NAMREPLY           "353"
# define RPL_MODE               "324"
# define RPL_TOPIC              "332"
# define RPL_WELCOME            "001"
# define RPL_NOTOPIC            "331"
# define RPL_INVITING           "341"
# define RPL_YOUREOPER          "381"
# define RPL_CHANNELMODES       "324"
# define RPL_ENDOFBANLIST       "368"
# define ERR_NORULES	        "434"
# define ERR_NOPRIVILEGES       "481"
# define ERR_NICKNAMEINUSE      "433"
# define ERR_NICKCOLLISION      "436"
# define ERR_NEEDMOREPARAMS     "461"
# define ERR_INVITEONLYCHAN     "473"
# define ERR_PASSWDMISMATCH     "464"
# define ERR_NONICKNAMEGIVEN    "431"
# define ERR_USERNOTINCHANNEL   "441"
# define ERR_CHANOPPRIVNEEDED   "482"
# define ERR_ERRONEUSNICKNAME	"432"
# define ERR_ALREADYREGISTERED  "462"
# define ERR_TOOMANYCHANNELS    "405"

class Channel;
class User{
    protected :
        std::string userName;
        std::string userNick;
        std::string userHostName;
        std::string userFullName;
        std::string userPass;
    public :
        bool                        userAuthentified;
        int                         sendFd;
        int                         primer;
        Channel                     *currentChannel;
        Channel                     *nextChannel;
        std::string                 serverName;
        std::vector<std::string>    commandFull;
    public :
        User();
        User(std::string name, std::string nick, std::string userFullName, std::string userHostName);
        User(User const &s);
        virtual ~User();
        User const  &operator=(User const & s);
        void    getCommands(std::string	buffer);
    /*-----------------------------Setters------------------------------------*/
    public :
        void    setNick(std::string const s);
        void    setNick(std::string const s, int mode);
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
    /*-------------------------**Member-Functions----------------------------*/

};

#endif