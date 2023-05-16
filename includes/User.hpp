#ifndef USER_HPP 
#define USER_HPP

#include <iostream>
#include <sstream>
#include <sys/socket.h>

class   User{
    protected:
        std::string User_Name;
        std::string User_Nick;
        std::string User_Channel;
        bool        Chanop_Permission;
    public:
        User();
        User(std::string name, std::string nick);
        ~User();
        User const & operator=(User const & s);
        User(User const & s);
        std::string get_User(){
            return (User_Nick);
        }
};

#endif