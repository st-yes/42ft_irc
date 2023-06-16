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
    this->serverName = "Banana kat3oum";
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
    //Server::sendInstructions(clientSocket);
    std::cout << "client trying to connect :" << clientSocket << std::endl;
}

void    Server::oConnection(int i)
{
    int             receivedBytes;
    char            buffer[BUFFER_SIZE];
    
    std::memset(buffer, 0, sizeof(buffer));
    receivedBytes = recv(this->pollers[i].fd, buffer, sizeof(buffer) - 1, 0);
    if (receivedBytes == -1 || receivedBytes == 0)
    //     throw errorErrno();
    // else if (receivedBytes == 0)
    {
        lostConnection(this->pollers[i].fd, i);
        return;
    }
    else
    {
        if (!this->users.find(this->pollers[i].fd)->second->userAuthentified){
            if (buffer[0] == '\n')
                Server::sendInstructions(this->pollers[i].fd);
            Server::fConnection(i, buffer, this->users.find(this->pollers[i].fd)->second);
        } //first connection?
        else
            Server::regularConnection(buffer, i);
    }
    return ;
}

void    Server::fConnection(int i, char *buffer, User *UserX)
{
    UserX->primer *= searchForCredentials(buffer, UserX);
    if (!UserX->userAuthentified && (UserX->primer == 5 || UserX->primer == 7 || UserX->primer == 35)){
        std::string* params = new std::string[3];
        params[0] = UserX->userNick;
        params[1] = ":Password should be given first!";
        params[2] = "";
        this->sendReply(this->pollers[i].fd, "localhost", ERR_PASSWDMISMATCH, params);
        delete[] params;
    }
    else if (!UserX->userAuthentified && UserX->primer == 105)
    {
        //std::cout << "pass =" << UserX->userPass << std::endl;
        //std::cout << "nick =" << UserX->userNick << std::endl;
        //std::cout << "user =" << UserX->userCommand << std::endl;
        if (!this->passCorrect(UserX->userPass)){
            //std::cout << "There you go homie  :" << UserX->userPass << std::endl;
            std::string* params = new std::string[3];
            params[0] = UserX->userNick;
            params[1] = ":Incorrect Password";
            params[2] = "";
            this->sendReply(this->pollers[i].fd, "localhost", ERR_PASSWDMISMATCH, params);
            delete[] params;
        }
        else if (this->nickAlreadyInUse(UserX->userNick, i))
        {
			std::string* params = new std::string[4];
			params[0] = "*";
			params[1] = UserX->userNick;
			params[2] = ":Nickname is already in use";
			params[3] = "";
			this->sendReply(this->pollers[i].fd,"localhost", ERR_NICKNAMEINUSE, params);
			delete[] params;
		}
        else
        {
            UserX->userAuthentified = true;
            this->sendWelcome(this->pollers[i].fd, UserX);
        }
    }
    return;
}

void    Server::regularConnection(std::string lebuffer, int i)
{
    //std::cout << "what is happening in here! " << lebuffer << std::endl;
    // for(int k = 0; k < this->pollers.size(); k++)
    // {
    //     if (this->pollers[k].fd == this->servSocketFd || i == k)
    //         continue;
    //     if (this->users.find(this->pollers[k].fd)->second->userAuthentified){
    //         if (send(this->pollers[k].fd, lebuffer.c_str(), lebuffer.length(), 0) == -1)
    //             throw errorErrno();}
    //     else
    //         continue; 
    // }
    this->sendEveryOne(lebuffer, i);
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

    //std::cout << it->second->getNick() << " has been disconnected!" << std::endl;
    //if (send(fd, str.c_str(), str.length(), 0) == -1)
     //   throw errorErrno();
    this->pollers.erase(this->pollers.begin() + i);
    std::cout << it->second->userNick + " " << fd << std::endl;
    this->users.erase(it);
    //for (int k = 1; k < this->pollers.size(); k++){
    //    if (send(this->pollers[k].fd, strII.c_str(), strII.length(), 0) == -1)
    //        throw errorErrno();
    //}
    close(fd);
}