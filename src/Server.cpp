
# include "Server.hpp"

Server::Server(){}
Server::Server(int s, std::string p) : serverName("Banana kat3oum"), numberOfCli(0), passWord(p), portNumber(s){
    std::cout <<  serverName + " is up! :)" << std::endl;
}
Server::Server(Server const &p){
    this->numberOfCli = p.numberOfCli;
    this->passWord = p.passWord;
    this->portNumber = p.portNumber;
    this->users  = p.users;
}
Server::~Server(){
    std::cout << "The server is down! :c" << std::endl;
}
Server  const &Server::operator=(Server const &p){
    *this = p;
    return *this;
}
void    Server::createConnection(){
    sockaddr_in6    servSocketAddr;
    sockaddr        *socketAddrBind;
    sockaddr        *cliAddrBind;
    pollfd          serverPoll;
    int             fl = 1;

    this->servSocketFd = socket(AF_INET6, SOCK_STREAM, 0);
    if (this->servSocketFd == -1)
        errnoCheck("socket()");
    memset(&servSocketAddr, 0, sizeof(servSocketAddr));
    servSocketAddr.sin6_family = AF_INET6;
    servSocketAddr.sin6_port = htons(this->portNumber);
    socketAddrBind = reinterpret_cast<sockaddr*>(&servSocketAddr);
    if (setsockopt(this->servSocketFd, SOL_SOCKET, SO_REUSEADDR, &fl, sizeof(int)) < 0)
        errnoCheck("setsockopt()");
    if (fcntl(this->servSocketFd, F_SETFL, O_NONBLOCK))
        errnoCheck("fcntl()");
    if (bind(this->servSocketFd, socketAddrBind, sizeof(servSocketAddr)) == -1)
        errnoCheck("bind()");
    std::cout << "The server is now available on the port : " << this->portNumber << std::endl;
    if (listen(this->servSocketFd, BACKLOG))
        errnoCheck("listen()");
    serverPoll.fd = this->servSocketFd;
    serverPoll.events = POLLIN;
    this->pollers.push_back(serverPoll);
    this->numberOfCli++;
    while (222)
        Server::Authentication();
}

void    Server::Authentication(){
    if (poll(this->pollers.data(), this->pollers.size(), 0) < 0)
        errnoCheck("poll()");
    for(size_t i = 0; i < this->pollers.size(); i++){
        if (this->pollers[i].revents & POLLIN){
            if (this->pollers[i].fd == this->servSocketFd)
                nConnection();
            else
                oConnection(i); // start listening through already existing connection!
        }
        else
            continue;
    }
}

void    Server::nConnection(){
    sockaddr_in6    clientSocketAddr;
    socklen_t       clientSocketSize = sizeof(clientSocketAddr);
    sockaddr        *cliAddrBind;
    pollfd          pollCli;
    User            *newU = NULL;
    int             clientSocket;

    memset(&pollCli, 0, sizeof(pollCli));
    cliAddrBind = reinterpret_cast<sockaddr*>(&clientSocketAddr);
    clientSocket = accept(this->servSocketFd, cliAddrBind, &clientSocketSize);
    fcntl(clientSocket, F_SETFL, O_NONBLOCK);
    if (clientSocket < 0)
        errnoCheck("accept()");
    pollCli.fd = clientSocket;
    pollCli.events = POLLIN;
    this->pollers.push_back(pollCli);
    newU = new User;
    this->users.insert(std::make_pair(clientSocket, newU));
    Server::sendInstructions(clientSocket);
    std::cout << "here it is my nigga!" << clientSocket << std::endl;
    return ;
}

void    Server::oConnection(int i){
    int             receivedBytes;
    char            buffer[BUFFER_SIZE];
    std::string     welcomeMsg = "Welcome Homie!\r\n";
    
    std::memset(buffer, 0, sizeof(buffer));
    receivedBytes = recv(this->pollers[i].fd, buffer, sizeof(buffer) - 1, 0);
    if (receivedBytes == -1)
        errnoCheck("recv()");
    else if (receivedBytes == 0){
        lostConnection(this->pollers[i].fd, i);
        return;
    }
    else{
        if (!this->users.find(this->pollers[i].fd)->second->userAuthentified)
            Server::fConnection(i, buffer, this->users.find(this->pollers[i].fd)->second);
        else
            Server::regularConnection(buffer, i);
    }
    return ;
}

void    Server::fConnection(int i, char *buffer, User *UserX){
    int primeNumber;

    primeNumber = searchForCredentials(buffer, UserX, 6);
    if (!UserX->userAuthentified && (primeNumber == 5 || primeNumber == 7 || primeNumber == 35)){
        std::string* params = new std::string[3];
        params[0] = UserX->userNick;
        params[1] = ":Password should be given first!";
        params[2] = "";
        this->sendReply(this->pollers[i].fd, "localhost", ERR_PASSWDMISMATCH, params);
        delete[] params;
    }
    else if (!UserX->userAuthentified && primeNumber == 105){
        std::cout << "pass =" << UserX->userPass << std::endl;
        std::cout << "nick =" << UserX->userNick << std::endl;
        std::cout << "user =" << UserX->userCommand << std::endl;
        if (!this->passCorrect(UserX->userPass)){
            std::string* params = new std::string[3];
            params[0] = UserX->userNick;
            params[1] = ":Incorrect Password";
            params[2] = "";
            this->sendReply(this->pollers[i].fd, "localhost", ERR_PASSWDMISMATCH, params);
            delete[] params;
        }
        else if (this->nickAlreadyInUse(UserX->userNick, i)){
			std::string* params = new std::string[4];
			params[0] = "*";
			params[1] = UserX->userNick;
			params[2] = ":Nickname is already in use";
			params[3] = "";
			this->sendReply(this->pollers[i].fd,"localhost", ERR_NICKNAMEINUSE, params);
            //std::cout << buffer << std::endl;
			//irssi give new nickname, i should acknowledge it
			delete[] params;
		}
        else{
            this->sendWelcome(this->pollers[i].fd, UserX);
            UserX->userAuthentified = true;
        }
    }
    return;
}

void    Server::regularConnection(char *buffer, int i){
    for(int i = 0; i < this->pollers.size(); i++){
        if (this->pollers[i].fd == this->servSocketFd)
            continue;
        if (send(this->pollers[i].fd, buffer, sizeof(buffer), 0) == -1)
            errnoCheck("send()");
    }
}

// User    *Server::createUser(){
//     User    object("banana", "frkouss", "hopla", "sinfoupli");
//     return &object;
// }

void    Server::lostConnection(int fd, int i){
    std::map<int, User*>::iterator it = this->users.find(fd);
    if (it == this->users.end()){
        std::cerr << "Theres something weird going on!" << std::endl;
        return;
    }
    std::vector<int>::iterator     vit;
    std::string                    str = "|You have been disconnected from the server|\r\n";
    std::string                    strII = it->second->getNick() + "Has left the server!\r\n";

    std::cout << it->second->getNick() << "has been disconnected!" << std::endl;
    if (send(fd, str.c_str(), str.length(), 0) == -1)
        errnoCheck("send()");
    this->pollers.erase(this->pollers.begin() + i);
    this->users.erase(it);
    for (int k = 1; k < this->pollers.size(); k++){
        if (send(this->pollers[k].fd, strII.c_str(), strII.length(), 0) == -1)
            errnoCheck("send()");
    }
    close(fd);
}