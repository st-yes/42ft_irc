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
    for (int i = 0; i < 3; i++){
        switch(i){
            case 0 : {
                chanDefault = new Channel("#Lobby!", "This aint no Therapy but go ahead and talk your shit away!"); 
            break;
            }
            case 1 : { // Case 1 and 2 will be deleted as soon as the testing is done! Default one to stay is case 0;
                chanDefault = new Channel("#9endahar!", "Some place far far away!");
            break;
            }
            case 2 : {
                chanDefault = new Channel("#Brkouksh", "Where Skafandri lives! I dont really know im out of weird topics!");
            break;
            }
        }
        this->servChannels.push_back(chanDefault);
    }
    
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
    if (setsockopt(this->servSocketFd, SOL_SOCKET, SO_REUSEADDR, &fl, sizeof(int)) == -1)
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
    if (clientSocket < 0)
        throw errorErrno();
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
        throw errorErrno();
    pollCli.fd = clientSocket;
    pollCli.events = POLLIN;
    this->pollers.push_back(pollCli);
    this->numberOfCli++;
    newU = new User;
    newU->userAuthentified = false;
    newU->primer = 1;
    this->users.insert(std::make_pair(clientSocket, newU));
    Server::sendInstructions(clientSocket);
    std::cout << "client trying to connect :" << clientSocket << std::endl;
}

void    Server::oConnection(int i)
{
    int             receivedBytes;
    char            buffer[BUFFER_SIZE];
    
    std::memset(buffer, 0, sizeof(buffer));
    receivedBytes = recv(this->pollers[i].fd, buffer, sizeof(buffer) - 1, 0);
    if (receivedBytes == -1 || receivedBytes == 0)
    {
        lostConnection(this->pollers[i].fd, i);
        return;
    }
    else
    {
        buffer[receivedBytes] = '\0';
        if (DEBUG)
        {
            // std::cout << "--------"<< std::endl;
            // std::cout << buffer;
            // std::cout << "--------"<< std::endl;
        }
        if (!this->users.find(this->pollers[i].fd)->second->userAuthentified)
        {
                std::cout << "----- First connection -----" << std::endl;
                Server::firstConnection(i, buffer, this->users.find(this->pollers[i].fd)->second); //first connection?
               this->users.find(this->pollers[i].fd)->second->userAuthentified = true;
               this->sendWelcome(this->pollers[i].fd, this->users.find(this->pollers[i].fd)->second);
               this->defaultChannelsAdd(this->pollers[i].fd, this->users.find(this->pollers[i].fd)->second);
        }
        else
           {
                std::cout << "----- Regular connection -----" << std::endl;
                Server::regularConnection(buffer, i, this->users.find(this->pollers[i].fd)->second);
           }
    }
    return ;
}



void    Server::regularConnection(std::string buffer, int i, User *UserX)
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
    this->sendEveryOne(buffer, i);
}

void    Server::sendEveryOne(std::string buffer, int i){
        for(int k = 0; k < this->pollers.size(); k++)
    {
        if (this->pollers[k].fd == this->servSocketFd || i == k)
            continue;
        if (this->users.find(this->pollers[k].fd)->second->userAuthentified){
            if (send(this->pollers[k].fd, buffer.c_str(), buffer.length(), 0) == -1)
                throw errorErrno();}
        else
            continue; 
    }
}

/*
* reply message when client lose connection 
*/
void    Server::lostConnection(int fd, int i)
{
    std::map<int, User*>::iterator it = this->users.find(fd);
    if (it == this->users.end())
    {
        std::cerr << "There's something weird going on!" << std::endl;
        return;
    }
    std::vector<int>::iterator     vit;
    std::string                    str = "|You have been disconnected from the server|\r\n";
    std::string                    strII = it->second->getNick() + " Has left the server!\r\n";


    this->pollers.erase(this->pollers.begin() + i);
    //std::cout << it->second->userNick + " " << fd << std::endl;
    this->users.erase(it);
    close(fd);
}

void    Server::defaultChannelsAdd(int fd, User *user){
    std::vector<Channel *>::iterator    it;
    Channel                             *current;
    int                                 index;
    int                                 index1 = -1;
    int                                 index2 = -1 ;
    for (index = 0; index != this->servChannels.size(); index++){
        if (this->servChannels[index]->channelName == "#Lobby!")
            break;
        else if (this->servChannels[index]->channelName == "#9andahar!")
            index1 = index;
        else if (this->servChannels[index]->channelName == "#Brkouksh!")
            index2 = index;
    }
    if (index != servChannels.size()){
        this->servChannels[index]->channelMembers.push_back(user);
        user->currentChannel = this->servChannels[index];
        user->nextChannel = this->servChannels[index];
       // if (this->servChannels[index]->channelIndex != index)
       //     this->servChannels[index]->channelIndex = index;
        current = this->servChannels[index];
    }
    else{
        if (index1 != -1){
            this->servChannels[index1]->channelMembers.push_back(user);
            user->currentChannel = this->servChannels[index1];
            user->nextChannel = this->servChannels[index1];
           // if (this->servChannels[index1]->channelIndex != index1)
           //     this->servChannels[index1]->channelIndex = index1;
            current = this->servChannels[index];
        }
        else if (index2 != -1){
            this->servChannels[index2]->channelMembers.push_back(user);
            user->currentChannel = this->servChannels[index2];
            user->nextChannel = this->servChannels[index2];
       //     if (this->servChannels[index2]->channelIndex != index2)
       //         this->servChannels[index2]->channelIndex = index2;
            current = this->servChannels[index];
        }
        else // add error!
            return;
    }
    this->sendWelcome(fd, user, current);
    return ;
}

void    Server::sendWelcome(int clientFd, User *user, Channel *current){
    std::string msg;
    std::string msg1;

    msg = ": You have joined the following channel :" + current->channelName + " \nThe channel Topic is : " + current->channelTopic + "\r\n";;
	if (send(clientFd, msg.c_str(), msg.length(), 0) == -1)
		throw errorErrno();
    msg1 = "The existing Channels are : \n";
    for (int i = 0; i != this->servChannels.size(); i++){
        msg1 += "- " + this->servChannels[i]->channelName;
        if (this->servChannels[i]->channelName == current->channelName)
            msg1 += " (You are here!) ";
        msg1 += "\r\n";
        if (clientFd == this->servSocketFd)
        continue;
    }
    if (send(clientFd, msg1.c_str(), msg1.length(), 0) == -1)
        throw errorErrno();
    msg = user->userNick + " has joined the channel!\r\n";
    for (int i = 0; i < this->pollers.size(); i++)
	{
        if (this->pollers[i].fd == this->servSocketFd || this->pollers[i].fd == clientFd)
            continue;
		if (authenticated(this->pollers[i].fd))
			if (send(this->pollers[i].fd, msg.c_str(), msg.length(), 0) == -1)
				throw errorErrno();
    }
}