#ifndef SERVER_HPP
#define SERVER_HPP

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include "User.hpp"
#include "commands.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>

class Server{
    protected :
        int numOfCli; // Store the number of connected Users on the Serv
        std::map<int, User*>Users; // Store the User Objects and the FDs related to their sockets.
        int port; // port number used for Serv communication
        std::string password; // Password used to allow connection.
    public :
        Server(int s, std::string p); // bla bla bla bla
        Server(Server const &s); // bla bla bla bla
        ~Server(); // bla bla bla bla
        Server const &operator=(Server const &s); // bla bla bla bla
        void    createConnection(); // Creating & connecting a socket both on the server and client side
        User   *createUser(int fd); // Just wanted to test with a User.
        bool    checkPassword(int fd); // Checking Passcode connection.
        
};





#endif