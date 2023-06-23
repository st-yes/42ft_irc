#ifndef IRCGOD_HPP
#define IRCGOD_HPP

# include "User.hpp"
# include "Server.hpp"
# include <map>

class Server;
class ircGod : public User{
    protected :
        Server      *servAccess;
    public :
        std::string godName;
    public :
        ircGod();
        ircGod(User const *s, std::string name);
        ircGod(ircGod const &s);
        ~ircGod();
        ircGod const &operator=(ircGod const &s); 
    public :
        Server *getGodPower();
        void    setGodPower(Server *s);
};



#endif