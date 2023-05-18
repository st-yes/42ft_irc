#include "Server.hpp"

Server::Server(int p, int s)
{
    this->NumofCli = 0;
    this->password = s;
    this->port = p;
    // std::cout << "Server Available!" << std::endl;
}

Server::~Server(){
    std::cout << "Server no longer available!" << std::endl;
}

Server const & Server::operator=(Server const &p){
    *this = p;
    return *this;
}

Server::Server(Server const &p){
    this->NumofCli = p.NumofCli;
    this->password  = p.password;
    this->port = p.port;
    this->Users = p.Users;
}

void    Server::Create_Connection(){
    int listen_socket = socket(AF_INET6, SOCK_STREAM, 0); // Creating a Socket, Used IPv6.
    if (listen_socket == -1)
        errorExit(SOCKET_FD);
    std::cout << "Server Available!" << std::endl;
    sockaddr_in6 sock_Addr; // Struct to store the connection infos.
    bzero(&sock_Addr, sizeof(sock_Addr)); // initializing the struct.
    sock_Addr.sin6_family = AF_INET6; // IPv6
    sock_Addr.sin6_port = htons(this->port); // making sure the port used will be read in big Endian.
    if (bind(listen_socket, (sockaddr *)&sock_Addr, sizeof(sock_Addr)) < 0){ // Binding the Socket to the port to listen to any incoming connections
        std::cerr << "Binding Error 1" << std::endl;
        exit (1);
    }
    if (listen(listen_socket, 1) < 0){ // listening for any connections.
        std::cerr << "Could not listen !" << std::endl;
        exit(1);
    }
    sockaddr_in6    sock_AddrCLI; // Struct to store Client connection infos
    socklen_t cli = sizeof(sock_AddrCLI);
    bzero(&sock_AddrCLI, cli); // initing the struct
    int client_sock = accept(listen_socket, (sockaddr *)&sock_AddrCLI,  &cli); // Accepting the connection on the port and allocating a socket (client_sock) to recieve it
    //Connection With Password
    if (!check_passcode(client_sock)){ //Checking if the pass code is correct!
        shutdown(client_sock, SHUT_RDWR); //Shuting the server dowm
    }
    else
    {
        User    *p = Create_User(client_sock); // Wanted to test if i could create a user, this is bullshit no worries
        this->Users.insert(std::make_pair(client_sock, p)); // adding the client along with his socket id in the map
        std::string b = "Welcome to Banana Tasba7!\n"; // Welcome message
        char pass[500];
        send(client_sock, b.c_str(), b.length(), 0); // sending Welcome message to user.
        while (1){  // Recieving any messages.
                bzero(pass, 500);
                if (recv(client_sock, &pass, 499, 0) <= 0){ 
                    std::cerr << "Connection lost" << std::endl;
                    break;
                }
                std::cout << this->Users.at(client_sock)->get_User() << " : " << pass; // printing it to the screen
        }
    }
}

bool    Server::check_passcode(int fd){
    std::string s = "Please Enter The Connection Pass Code : ";
    std::string pass;
    bzero(&pass, sizeof(pass));
    if (send(fd, s.c_str(), s.length(), 0) < 0 )
    {
        std::cerr << "Could not send message!" << std::endl;
        return false;
    }
    if (recv(fd, &pass, 5, 0) <= 0){
        std::cerr << "Connection lost" << std::endl;
        return false;
    }
    try{
        if (stoi(pass) != this->password){
            std::cout << pass << std::endl;
            std::cout << this->password << std::endl;
            std::string b = "Wrong Password!\n";
            send(fd, &b, b.length(), 0);
            return false;
        }
        else
            return true;}
    catch(...){
        std::cerr << pass << "Some weird shit has been sent!" << std::endl;
    }
}

User*    Server::Create_User(int fd){
    User *u = new User("banana", "frkouss");
    return u;
}