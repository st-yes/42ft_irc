# include "../includes/Ft_irc.hpp"

Server::Server(){}

Server::Server(int ac, char **av)
{
    if (ac != 3)
        throw errorNbArguments();
    std::string portNum(av[1]);
    std::string pass(av[2]);
    if (!validPort(portNum))
        throw errorPort();
    if (!validPass(pass))
        throw errorPass(); 
    this->numberOfCli = 0;
    this->passWord = pass;
    this->portNumber = std::atoi(portNum.c_str());
    this->serverName = SERVERNAME;
    this->gods.insert(std::make_pair("Skafandri","22221111"));
    this->gods.insert(std::make_pair("Stoph", "k7el"));
    this->gods.insert(std::make_pair("SoukSouk", "pepe"));
    Channel *chanDefault = NULL;
    chanDefault = new Channel("#Lobby!", "This aint no Therapy but go ahead and talk your shit away!"); 
    this->servChannels.push_back(chanDefault);
}
Server::Server(Server const &p)
{
    this->numberOfCli = p.numberOfCli;
    this->passWord = p.passWord;
    this->portNumber = p.portNumber;
    this->users  = p.users;
}
Server::~Server(){}

Server  const &Server::operator=(Server const &p){
    *this = p;
    return *this;
}

/*
*
*/
void    Server::createConnection()
{
    sockaddr_in    servSocketAddr;
    sockaddr        *socketAddrBind;
    sockaddr        *cliAddrBind;
    pollfd          serverPoll;
    int             fl = 1;

    this->servSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->servSocketFd == -1)
        throw errorErrno();
    memset(&servSocketAddr, 0, sizeof(servSocketAddr));
    servSocketAddr.sin_family = AF_INET;
    servSocketAddr.sin_port = htons(this->portNumber);
    socketAddrBind = reinterpret_cast<sockaddr*>(&servSocketAddr);
    if (setsockopt(this->servSocketFd, SOL_SOCKET, SO_REUSEADDR, &fl, sizeof(int)) == -1)
        throw errorErrno();
    if (fcntl(this->servSocketFd, F_SETFL, O_NONBLOCK) == -1)
        throw errorErrno();
    if (bind(this->servSocketFd, socketAddrBind, sizeof(servSocketAddr)) == -1)
        throw errorErrno();
    std::cout << "The server is now available on the port : " << this->portNumber << std::endl;
    if (listen(this->servSocketFd, BACKLOG) == -1)
        throw errorErrno();
    serverPoll.fd = this->servSocketFd;
    serverPoll.events = POLLIN;
    this->pollers.push_back(serverPoll);
    while (true)
        Server::Authentication();
}

void    Server::Authentication()
{
    if (poll(this->pollers.data(), this->pollers.size(), 0) == -1)
        throw errorErrno();
    for (size_t i = 0; i < this->pollers.size(); i++)
    {
        if (this->pollers[i].revents & POLLIN)
        {
            if (this->pollers[i].fd == this->servSocketFd)
                this->nConnection(); //newConnection
            else
                this->oConnection(i); //old connection
        }
        else
            continue;
    }
}

void    Server::nConnection()
{
    sockaddr_in    clientSocketAddr;
    socklen_t       clientSocketSize = sizeof(clientSocketAddr);
    sockaddr        *cliAddrBind;
    pollfd          pollCli;
    User            *newU = NULL;
    int             clientSocket;

    memset(&pollCli, 0, sizeof(pollCli));
    cliAddrBind = reinterpret_cast<sockaddr*>(&clientSocketAddr);
    clientSocket = accept(this->servSocketFd, cliAddrBind, &clientSocketSize);
    if (clientSocket < 0) // need to be rechecked for error
        throw errorErrno();
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1) // need to be rechecked for error 
        throw errorErrno();
    pollCli.fd = clientSocket;
    pollCli.events = POLLIN;
    this->pollers.push_back(pollCli);
    this->numberOfCli++;
    newU = new User;
    newU->userAuthentified = false;
    newU->primer = 1;
    newU->passSet = false;
    newU->sendFd = clientSocket;
    // User *a = new User;
    // User *b = new User;
    // a->setNick("styes");
    // b->setNick("styes_");
    // this->users.insert(std::make_pair(7, a));
    // this->users.insert(std::make_pair(8, b)); 
    this->users.insert(std::make_pair(clientSocket, newU));
    Server::sendInstructions(clientSocket);
    std::cout << "client trying to connect :" << clientSocket << std::endl;
}

bool    Finished(std::string cmd)
{
    if (cmd.find("\n") != std::string::npos)
        return (true);
    return (false);
}

// void    Server::oConnection(int i)
// {
//     int             receivedBytes;
//     char            buffer[BUFFER_SIZE];
//     User            *currentUser = this->users.find(this->pollers[i].fd)->second;
//     int             paramNumber;
    
//     std::memset(buffer, 0, sizeof(buffer));
//     receivedBytes = recv(this->pollers[i].fd, buffer, sizeof(buffer) - 1, 0);
//     if (receivedBytes == -1 || receivedBytes == 0)
//     {
//         lostConnection(currentUser);
//         return;
//     }
//     else
//     {
//         buffer[receivedBytes] = '\0';
//         if (DEBUG)
//         {
//             std::cout << "--------"<< std::endl;
//             std::cout << buffer;
//             std::cout << "--------"<< std::endl;
//         }
//         currentUser->setNc(buffer);
//         if (currentUser->userAuthentified == false)
//         {
//                 std::cout << "----- First connection -----" << std::endl;
//                 this->firstConnection(i, buffer, currentUser); //first connection?
//         }
//         else if (currentUser->userAuthentified == true)
//            {
//                 std::cout << "----- Regular connection -----" << std::endl;
//                 this->regularConnection(buffer, currentUser);
//            }
//         else if (currentUser->userAuthentified == NICK_AGAIN)
//         {
//             std::string *cmdParams = getCmdParams(buffer, currentUser, &paramNumber);

//             if (cmdParams[0] == "NICK" || cmdParams[0] == "nick")
//                 this->handleCmdNickAgain(currentUser->sendFd, cmdParams, currentUser, paramNumber);
//             else if (cmdParams[0] == "USER" || cmdParams[0] =="user")
//             {
//                 currentUser->primer *= 7;
//                 this->handleCmdUser(cmdParams,currentUser, paramNumber);
//             }
//         }
//     }
//     return ;
// }

void    Server::oConnection(int i)
{
    int             receivedBytes;
    char            buffer[BUFFER_SIZE];
    User            *currentUser = this->users.find(this->pollers[i].fd)->second;
    int             paramNumber;
   static std::string     cmd;

    std::memset(buffer, 0, sizeof(buffer));
    receivedBytes = recv(this->pollers[i].fd, buffer, sizeof(buffer) - 1, 0);
    if (receivedBytes == -1 || receivedBytes == 0)
    {
        cmd = "";
        lostConnection(currentUser);
        return;
    }
    else
    {
        buffer[receivedBytes] = '\0';
        if (DEBUG)
        {
            std::cout << "--------"<< std::endl;
            std::cout << buffer;
            std::cout << "--------"<< std::endl;
        }
        currentUser->setNc(buffer);
        cmd += buffer;
        std::cout <<"cmd = " << cmd <<std::endl;
        if (cmd.find("\n") == std::string::npos)
           ;
        else if (currentUser->userAuthentified == false)
        {
                std::cout << "----- First connection -----" << std::endl;
                this->firstConnection(i, cmd, currentUser); //first connection?
                cmd = "";
        }
        else if (currentUser->userAuthentified == true)
           {
                std::cout << "----- Regular connection -----" << std::endl;
                this->regularConnection(cmd, currentUser);
                cmd = "";
           }
        else if (currentUser->userAuthentified == NICK_AGAIN)
        {
            std::string *cmdParams = getCmdParams(cmd, currentUser, &paramNumber);

            if (cmdParams[0] == "NICK" || cmdParams[0] == "nick")
                this->handleCmdNickAgain(currentUser->sendFd, cmdParams, currentUser, paramNumber);
            else if (cmdParams[0] == "USER" || cmdParams[0] =="user")
            {
                currentUser->primer *= 7;
                this->handleCmdUser(cmdParams,currentUser, paramNumber);
            }
             cmd = "";
        }
    }
    return ;
}

void    Server::regularConnection(std::string buffer, User *UserX)
{
    int		size;
	int		paramNumber;
	std::string	*cmdParams;
	std::string buff(buffer);

	UserX->getCommands(buff, true);
	size = UserX->commandFull.size();
	cmdParams = NULL;
	for (size_t j = 0; j < size; j++)
	{
		if (!successiveSpace(UserX->commandFull[j]))	
		{
			paramNumber = countParams(UserX->commandFull[j]);
			cmdParams =  fillParams(UserX->commandFull[j], paramNumber);
			this->handleOtherCmds(UserX, cmdParams, paramNumber - 1);
		}
	}
}
/*
* reply message when client lose connection 
*/
void    Server::lostConnection(User *user)
{
    std::vector<int>::iterator     vit;
    std::string                    str = "|You have been disconnected from the server|\r\n";
    std::string                    strII = user->getNickForReply() + " Has left the server!\r\n";
    this->deleteFromPoll(user->sendFd);
    this->users.erase(user->sendFd);
    close(user->sendFd);
    delete user;
}

void    Server::defaultChannelsAdd(User *user)
{
    Channel                             *current;
    std::vector<User*>               send;
    send.push_back(user);
    current = channelFinder("#Lobby!");
    if (current){
        current->channelMembers.push_back(user);
        user->joinedChannels.push_back(current);
        user->defaultChannel = current;
        //this->sendGenericReply(user, "JOIN", user->defaultChannel, "You joined the default channel, welcome!");
        this->sendHermes(this->sendGenericCode(user, current, "JOIN", "you have joined the default"), send);
    }
    else // add error!
        this->lostConnection(user);
    //this->sendWelcome(user, current);
    return ;
}
