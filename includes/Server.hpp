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
# include "Channel.hpp"
# include "ircGod.hpp"

class   Server
{
    protected :
        std::map<int, User*>                users;
        std::map<std::string, std::string>  gods;
        std::vector<pollfd>                 pollers;
        std::vector<Channel *>              servChannels;
        std::string                         passWord;
        std::string                         serverName;
        int                                 portNumber;
        int                                 numberOfCli;
        int                                 servSocketFd;
    public :
        Server();
        Server(int ac, char **av);
        Server(Server const &p);
        ~Server();
        Server  const &operator=(Server const &p);
    public :
        void    sendEveryOne(std::string buffer, int i);
        void    fConnection(int i, char *buffer, User *New);
        //void    regularConnection(std::string buffer, int i);
        void    regularConnection(std::string buffer, int i, User *UserX);
        void    createConnection();
        void    Authentication();
        void    nConnection();
        void    oConnection(int i);
        void    lostConnection(int fd, int i);
        void	sendInstructions(int clientFd);
        void	sendWelcome(int	clientFd, User *user);
        void    sendWelcome(int clientFd, User *user, Channel *current);
        int     findClientFd(User *userX);
        void    handleOtherCmds(User *UserX, std::string* cmdParams, int paramNumber);

        //int     searchForCredentials(std::string buffer, User *newUser);
        //void    fetchTheFirst(std::string command, std::string buffer, User *newUser);
        
        void    sendReply(int clientFd, std::string prefix, std::string numericCode, std::string *params);
        bool	passCorrect(std::string passUser);
        
        bool	nickAlreadyInUse(std::string nick);
        bool	authenticated(int fdClient);
        void    defaultChannelsAdd(int clientFd, User *user);

        void    firstConnection(int i, char *buffer, User *UserX);
        /*-- COMMANDS PART 1--*/
        void    handleAuthentCmds(User *UserX, std::string* cmdParams, int paramNumber);
        void	handleCmdPass(std::string	*params, User *userX, int paramNumber);
        void	handleCmdNick(std::string	*params, User *userX, int paramNumber);
        void	handleCmdUser(std::string	*params, User *userX, int paramNumber);
        /*-- COMMANDS PART 2--*/
        void    handleCmdOper(std::string   *params, User *userX, int paramNumber);

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