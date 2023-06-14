#ifndef SERVER_HPP
#define SERVER_HPP

# include <sstream>
# include <iostream>
# include <iomanip>
# include <cstring>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <map>
# include <vector>
# include <poll.h>
# include <fcntl.h>
# include "User.hpp"

class   Server
{
    protected :
        std::map<int, User*>    users;
        std::vector<pollfd>     pollers;
        std::string             passWord;
        std::string             serverName;
        int                     portNumber;
        int                     numberOfCli;
        int                     servSocketFd;
    public :
        Server();
        Server(int ac, char **av);
        Server(Server const &p);
        ~Server();
        Server  const &operator=(Server const &p);
    public :
        void    fConnection(int i, char *buffer, User *New);
        void    regularConnection(std::string buffer, int i);
        void    createConnection();
        void    Authentication();
        void    nConnection();
        void    oConnection(int i);
        void    lostConnection(int fd, int i);
        User    *createUser();
        void	sendInstructions(int clientFd);
        void	sendWelcome(int	clientFd, User *user);
        int     searchForCredentials(std::string buffer, User *newUser);
        void    fetchTheFirst(std::string command, std::string buffer, User *newUser);
        void    sendReply(int clientFd, std::string prefix, std::string numericCode, std::string *params);
        bool	passCorrect(std::string passUser);
        bool	nickAlreadyInUse(std::string nick, int i);
        bool	authenticated(int fdClient);

        /*-------------------EXCEPTIONS------------------*/
        

        class errorNbArguments : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
        class errorPort : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
        class errorPass : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
        class errorErrno : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
};

#endif