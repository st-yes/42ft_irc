#ifndef SERVER_HPP
#define SERVER_HPP

#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "User.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>

class Server{
    protected :
        int NumofCli; // Store the number of connected Users on the Serv
        std::map<int, User*>Users; // Store the User Objects and the FDs related to their sockets.
        int port; // port number used for Serv communication
        int password; // Password used to allow connection.
    public :
        Server(int s, int p); // bla bla bla bla
        Server(Server const &s); // bla bla bla bla
        ~Server(); // bla bla bla bla
        Server const &operator=(Server const &s); // bla bla bla bla
        void    Create_Connection(); // Creating & connecting a socket both on the server and client side
        User   *Create_User(int fd); // Just wanted to test with a User.
        bool    check_passcode(int fd); // Checking Passcode connection.
        
};





#endif