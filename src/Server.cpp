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
    this->gods  = p.gods;
    this->pollers = p.pollers;
    this->servChannels = p.servChannels;
    this->servSocketFd = p.servSocketFd;
    this->serverName = p.serverName;
}
Server::~Server(){}

Server  const &Server::operator=(Server const &p){
    *this = p;
    return *this;
}

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
                this->nConnection();
            else
                this->oConnection(i);
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
    if (clientSocket < 0){
        close(clientSocket);
        return ;
    }
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1){
        close(clientSocket);
        return ;
    }
    pollCli.fd = clientSocket;
    pollCli.events = POLLIN;
    this->pollers.push_back(pollCli);
    this->numberOfCli++;
    newU = new User;
    newU->userAuthentified = false;
    newU->primer = 1;
    newU->passSet = false;
    newU->sendFd = clientSocket;
    this->users.insert(std::make_pair(clientSocket, newU));
    std::cout << "client trying to connect :" << clientSocket << std::endl;
}

bool    Finished(std::string cmd)
{
    if (cmd.find("\n") != std::string::npos)
        return (true);
    return (false);
}

void    Server::oConnection(int i)
{
    int             receivedBytes;
    char            buffer[BUFFER_SIZE];
    User            *currentUser = this->users.find(this->pollers[i].fd)->second;
    int             paramNumber;
    static std::string     cmd;

    std::memset(buffer, 0, sizeof(buffer));
    receivedBytes = recv(this->pollers[i].fd, buffer, sizeof(buffer) - 1, 0);
    if (receivedBytes == -1 || receivedBytes == 0 || receivedBytes >= 512)
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
        if (cmd.find("\n") == std::string::npos)
           ;
        else if (currentUser->userAuthentified == false)
        {
                this->firstConnection(i, cmd, currentUser);
                cmd = "";
        }
        else if (currentUser->userAuthentified == true)
        {
            this->regularConnection(cmd, currentUser);
            cmd = "";
        }
        else if (currentUser->userAuthentified == NICK_AGAIN)
        {
            std::string *cmdParams = getCmdParams(cmd, currentUser, &paramNumber);

            if (validCmd("nick", cmdParams[0]))
                this->handleCmdNickAgain(currentUser->sendFd, cmdParams, currentUser, paramNumber);
            else if (validCmd("user", cmdParams[0]))
            {
                currentUser->primer *= 7;
                this->handleCmdUser(cmdParams,currentUser, paramNumber);
            }
            cmd = "";
            delete[] cmdParams;
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
// void    Server::lostConnection(User *user)
// {
//     for(int i = 0; i != user->joinedChannels.size(); i++){
//         for (int k = 0; k != user->joinedChannels[i]->channelMembers.size(); k++){
//             if (user->sendFd == user->joinedChannels[i]->channelMembers[k]->sendFd)
//               {
//                 user->joinedChannels[i]->channelMembers.erase(user->joinedChannels[i]->channelMembers.begin() + k);
//                 //=break;
//               }
//         }
//     }
//     this->deleteFromPoll(user->sendFd);
//     this->users.erase(user->sendFd);
//     close(user->sendFd);
//     delete user;
// }

//new lost connection
void Server::lostConnection(User *user)
{
    Channel* channel;

    for (int i = 0; i < user->joinedChannels.size(); i++)
    {
        channel = user->joinedChannels[i];
        for (int k = 0; k < channel->channelMembers.size(); ++k) {
            if (user->sendFd == channel->channelMembers[k]->sendFd) {
                channel->channelMembers.erase(channel->channelMembers.begin() + k);
                break;
            }
        }
    }
    this->deleteFromPoll(user->sendFd);
    std::map<int, User*>::iterator it = this->users.find(user->sendFd);
    if (it != this->users.end()) {
        this->users.erase(it);
    }
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
        this->sendHermes(this->sendGenericCode(user, current, "JOIN", "you have joined the default"), send);
    }
    else
        this->lostConnection(user);
    return ;
}
