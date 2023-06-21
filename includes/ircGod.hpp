#ifndef IRCGOD_HPP
#define IRCGOD_HPP

# include "User.hpp"
# include "Server.hpp"
# include <map>


class ircGod : private User{
    protected :
        std::string godName;
        Server      *servAccess;
    public :
        ircGod();
        ircGod(User const &s, std::string name);
        ircGod(ircGod const &s);
        ~ircGod();
        ircGod const &operator=(ircGod const &s); 
};



#endif