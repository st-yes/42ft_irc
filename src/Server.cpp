# include "Ft_irc.hpp"

Server::Server(){}
// Server::Server(int s, std::string p) : serverName("Banana kat3oum"), numberOfCli(0), passWord(p), portNumber(s){
//     //std::cout <<  serverName + " is up! :)" << std::endl;
// }
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
    this->gods.insert(std::make_pair("SoukSouk", ""));
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
Server::~Server()
{
    //std::cout << "The server is down! :c" << std::endl;
}

Server  const &Server::operator=(Server const &p){
    *this = p;
    return *this;
}

/*
*
*/
void    Server::createConnection()
{
    sockaddr_in6    servSocketAddr;
    sockaddr        *socketAddrBind;
    sockaddr        *cliAddrBind;
    pollfd          serverPoll;
    int             fl = 1;

    this->servSocketFd = socket(AF_INET6, SOCK_STREAM, 0);
    if (this->servSocketFd == -1)
        throw errorErrno();
    memset(&servSocketAddr, 0, sizeof(servSocketAddr));
    servSocketAddr.sin6_family = AF_INET6;
    servSocketAddr.sin6_port = htons(this->portNumber);
    socketAddrBind = reinterpret_cast<sockaddr*>(&servSocketAddr);
    if (setsockopt(this->servSocketFd, SOL_SOCKET, SO_REUSEPORT, &fl, sizeof(int)) == -1)
        throw errorErrno();
    if (fcntl(this->servSocketFd, F_SETFL, O_NONBLOCK) == -1)
        throw errorErrno();
    if (bind(this->servSocketFd, socketAddrBind, sizeof(servSocketAddr)) == -1)
        throw errorErrno();
    //std::cout << "The server is now available on the port : " << this->portNumber << std::endl;
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
    sockaddr_in6    clientSocketAddr;
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
    newU->sendFd = clientSocket;
    newU->currentChannel = NULL;
    newU->nextChannel = NULL;
    this->users.insert(std::make_pair(clientSocket, newU));
    Server::sendInstructions(clientSocket);
    std::cout << "client trying to connect :" << clientSocket << std::endl;
}

void    Server::oConnection(int i)
{
    int             receivedBytes;
    char            buffer[BUFFER_SIZE];
    User            *currentUser = this->users.find(this->pollers[i].fd)->second;
    
    std::memset(buffer, 0, sizeof(buffer));
    receivedBytes = recv(this->pollers[i].fd, buffer, sizeof(buffer) - 1, 0);
    if (receivedBytes == -1 || receivedBytes == 0)
    {
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
        if (!currentUser->userAuthentified)
        {
                std::cout << "----- First connection -----" << std::endl;
                Server::firstConnection(i, buffer, currentUser); //first connection?
                currentUser->userAuthentified = true;
               this->sendWelcome(currentUser);
               this->defaultChannelsAdd(currentUser);
        }
        else
           {
                std::cout << "----- Regular connection -----" << std::endl;
                Server::regularConnection(buffer, currentUser);
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

	UserX->getCommands(buff);
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
    this->sendEveryOne(buffer, UserX);
}

void    Server::sendEveryOne(std::string buffer, User *currentUser){
        for(int k = 0; k < this->pollers.size(); k++)
    {
        if (this->pollers[k].fd == this->servSocketFd || this->pollers[k].fd == currentUser->sendFd)
            continue;
        if (this->users.find(this->pollers[k].fd)->second->userAuthentified){
            if (send(this->pollers[k].fd, buffer.c_str(), buffer.length(), 0) == -1)
                throw errorErrno();} // recheck for error
        else
            continue; 
    }
}

/*
* reply message when client lose connection 
*/
void    Server::lostConnection(User *user)
{
    std::vector<int>::iterator     vit;
    std::string                    str = "|You have been disconnected from the server|\r\n";
    std::string                    strII = user->getNick() + " Has left the server!\r\n";
    this->deleteFromPoll(user->sendFd);
    this->users.erase(user->sendFd);
    close(user->sendFd);
}

void    Server::defaultChannelsAdd(User *user){
    std::vector<Channel *>::iterator    it;
    Channel                             *current;
    int                                 index;
    for (index = 0; index != this->servChannels.size(); index++){
        if (this->servChannels[index]->channelName == "#Lobby!")
            break;
    }
    if (index != servChannels.size()){
        this->servChannels[index]->channelMembers.push_back(user);
        user->currentChannel = this->servChannels[index];
        user->nextChannel = this->servChannels[index];
        current = this->servChannels[index];
    }
    else // add error!
        return;
    this->sendWelcome(user, current);
    return ;
}

void    Server::sendWelcome(User *user, Channel *current){
    std::string msg;
    std::string msg1;

    msg = ": You have joined the following channel :" + current->channelName + " \nThe channel Topic is : " + current->channelTopic + "\r\n";;
	if (send(user->sendFd, msg.c_str(), msg.length(), 0) == -1)
		throw errorErrno(); // check error
    msg1 = "The existing Channels are : \n";
    for (int i = 0; i != this->servChannels.size(); i++){
        msg1 += "- " + this->servChannels[i]->channelName;
        if (this->servChannels[i]->channelName == current->channelName)
            msg1 += " (You are here!) ";
        msg1 += "\r\n";
        if (user->sendFd == this->servSocketFd)
            continue;
    }
    if (send(user->sendFd, msg1.c_str(), msg1.length(), 0) == -1)
        throw errorErrno(); // check error
    msg = user->getNick() + " has joined the channel!\r\n";
    for (int i = 0; i < this->pollers.size(); i++)
	{
        if (this->pollers[i].fd == this->servSocketFd || this->pollers[i].fd == user->sendFd)
            continue;
		if (authenticated(this->pollers[i].fd))
			if (send(this->pollers[i].fd, msg.c_str(), msg.length(), 0) == -1)
				throw errorErrno(); // check error
    }
}