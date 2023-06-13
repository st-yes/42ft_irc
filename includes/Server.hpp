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
# include "helperFunctions.hpp"

class   Server{
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
        Server(int s, std::string p);
        Server(Server const &p);
        ~Server();
        Server  const &operator=(Server const &p);
    public :
        void    fConnection(int i, char *buffer, User *New);
        void    regularConnection(char *buffer, int i);
        void    createConnection();
        void    Authentication();
        void    nConnection();
        void    oConnection(int i);
        void    lostConnection(int fd, int i);
        User    *createUser();
        void	sendInstructions(int clientFd);
        void	sendWelcome(int	clientFd, User *user);
        void	sendNumericReply(const std::string& prefix, const std::string& replyCode,
 std::string& nick, const std::string& message, int clientFd);
        void	sendNumericReplyCommand(const std::string& prefix, const std::string& replyCode,
 std::string nick,const std::string& command, const std::string& message, int clientFd);

        int     searchForCredentials(std::string buffer, User *newUser, int o);
        void    fetchTheFirst(std::string command, std::string buffer, User *newUser);
        void    sendReply(int clientFd, std::string prefix, std::string numericCode, std::string *params);
        bool	passCorrect(std::string passUser);
        bool	nickAlreadyInUse(std::string nick, int i);

    //add exeptions
    class ProblemInFdServer : public std::exception
    {
        public:
            virtual const char *what() const throw();
    };
};

#endif