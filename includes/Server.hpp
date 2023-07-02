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
# include <unordered_map>
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
        void    sendEveryOne(std::string buffer, User *currentUser);
        void    regularConnection(std::string buffer, User *UserX);
        void    createConnection();
        void    Authentication();
        void    nConnection();
        void    oConnection(int i);
        void    lostConnection(User *user);
        void	sendInstructions(int clientFd);
        void	sendWelcome(User *user);
        void	sendHermes(std::string reply, std::vector<User *> recipients);
        std::string	sendNumericCode(User *userX, Channel* channel, std::string numericCode, std::string lines);
        std::string	sendGenericCode(User *userX, Channel *chan, std::string prefix, std::string lines);
        void    sendWelcome(User *user, Channel *current);
        void    handleOtherCmds(User *UserX, std::string* cmdParams, int paramNumber);
        void    sendReply(int clientFd, std::string prefix, std::string numericCode, std::string *params);
        bool	passCorrect(std::string passUser);
        bool	nickAlreadyInUse(std::string nick);
        bool	authenticated(int fdClient);
        void    defaultChannelsAdd(User *user);
        void    firstConnection(int i, std::string buffer, User *UserX);
        /*-- COMMANDS PART 1--*/
        void    handleAuthentCmds(User *UserX, std::string* cmdParams, int paramNumber);
        void	handleCmdPass(std::string	*params, User *userX, int paramNumber);
        void	handleCmdNick(std::string	*params, User *userX ,int paramNumber);
        void	handleCmdUser(std::string	*params, User *userX, int paramNumber);
        void	handleCmdNickAgain(int clientFd, std::string	*params, User *userX ,int paramNumber);
        void    handleCmdPing(std::string *s, User *a, int b);
        /*-- COMMANDS PART 2--*/
        void    handleCmdOper(std::string   *params, User *userX, int paramNumber);
        void    handleCmdMode(std::string   *params, User *userX, int paramNumber);
        void    handleCmdModeOpt(std::string *params, User *userX, Channel *chan);
        void    handleCmdModeOpt(Channel *chan, User *userX, std::map<char,std::string> opt, int mode);
        void    handleCmdModeOptI(User *userX, Channel *chan, int mode);
        void    handleCmdModeOptT(User *userX, Channel *chan, int mode);
        void    handleCmdModeOptO(User *userX, std::string s, Channel *chan, int mode);
        void    handleCmdModeOptK(User *userX, std::string s, Channel *chan, int mode);
        void    handleCmdModeOptL(User *userX, std::string s, Channel *chan, int mode);
        void    handleCmdTopic(std::string *params, User *userX, int paramNumber);
        /*-- COMMANDS PART 3--*/
        void        handleCmdJoin(std::string* str, User* Userx, int paramNumber);
        bool        syntaxCheck(char *p);
        void        JoinFunc(std::unordered_map<std::string, std::string>   tmp, User *userX);
        void        handleCmdPart(std::string *s, User *userX, int paramNumber);
        void        ParsePart(std::vector<std::string> &channel,std::string* str, std::string& reason);
        void        handleCmdKick(std::string *s, User *userX, int paramNumber);
        void        handleCmdInvite(std::string *params, User *userX, int paramNumber);
        void        newChannel(std::unordered_map<std::string, std::string>::iterator p, User *userX);
        std::string getChannelPriv(std::string s);
    
        /*-- HELPER-FUNCTIONS --*/
            int                     findUserinChan(int fd, std::vector<User *> x);
            User                    *findUserinServ(std::string name);
            User                    *findUserinServ(int fd);
            void                    deleteFromPoll(int fd);
            Channel                 *channelFinder(std::string s);
            bool                    channelFinder(std::string s, std::vector<Channel *> chans);
            int                     channelFinder2(std::string s, std::vector<Channel*> chan);
            void                    sendGenericReply(User *userX, std::string prefix, Channel *chan, std::string s); 
            void                    sendReply(int clientFd, std::string numericCode, std::string *params);
            void                    sendReplyNames(User *userX, std::string numericCode, Channel *chan);
            void	                sendStatusUpdate(int clientFd, User *userX, std::string com, std::string msg); 
            void                    handleCmdRegularNick(std::string	*params, User *userX, int paramNumber);
            void                    addNewChanops(User *userX, Channel *chan);
           // void    sendResponses(User *user, Channel *chan, std::string num, std::string s);
           std::vector<User *>      godFinder();
           void                     handleCmdMsg(std::string	*params, User *userX ,int paramNumber, int flag);
           int                      findClient(std::string	nick);
           std::vector<std::string> divideAndConquer(std::string s, User *userX);
           void                     handleCmdQuit(std::string	*params, User *userX ,int paramNumber);
           std::string              generateMsgPriv(User *userX, std::string	*params, int paramNumber, int flag);
            void                    bot(std::string	*params, User *userX ,int paramNumber);
            void                     handleMultiMsg(std::string	*params, User *userX ,int paramNumber, int flag);
            std::string              generateMsgPrivNc(User *userX, std::string	nick, std::string	msg, int flag);
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