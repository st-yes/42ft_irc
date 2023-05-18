
#include "Server.hpp"

Server::Server(int p, std::string s)
{
    this->numOfCli = 0;
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
    this->numOfCli = p.numOfCli;
    this->password  = p.password;
    this->port = p.port;
    this->Users = p.Users;
}

void	Server::createConnection()
{
	int				servSocket;
	sockaddr_in6	servSocketAddr;
	sockaddr		*socketAddrBind;
	
	int				clientSocket;
	sockaddr_in6	clientSocketAddr;
	sockaddr		*cliAddrBind;
	socklen_t		clientSocketSize;
	int				receivedBytes;
	int				tries = 3;
	bool			passwordPending = true;
	std::string	passwordRequest = "Please enter your password to authenticate: ";

	servSocket = socket(AF_INET6, SOCK_STREAM, 0);
	if (servSocket == -1)
		errnoCheck("socket()");
	memset(&servSocketAddr, 0, sizeof(servSocketAddr));
	servSocketAddr.sin6_family = AF_INET6;
    servSocketAddr.sin6_port = htons(this->port);
	socketAddrBind = reinterpret_cast<sockaddr*>(&servSocketAddr);
	if (bind(servSocket, socketAddrBind, sizeof(servSocketAddr)) == -1)
		errnoCheck("bind()");
	std::cout << "SERVER AVAILABLE" << std::endl;
	if (listen(servSocket, BACKLOG) == -1)
		errnoCheck("listen()");
	clientSocketSize = sizeof(clientSocketAddr);
	while (true)
	{
		cliAddrBind = reinterpret_cast<sockaddr*>(&clientSocketAddr);
		clientSocket = accept(servSocket, cliAddrBind, &clientSocketSize);
		if (clientSocket == -1)
			errnoCheck("accept()");
		char buffer[BUFFER_SIZE];
		while (true)
		{
			if (passwordPending == true)
			{
				if (tries == 0)
				{
					passwordRequest = "Access denied, leave.";
					if (send(clientSocket, passwordRequest.c_str(), passwordRequest.length(), 0) == -1)
						errnoCheck("send()");
					close (clientSocket);
					break;
				}
				if (send(clientSocket, passwordRequest.c_str(), passwordRequest.length(), 0) == -1)
					errnoCheck("send()");
				receivedBytes = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				if (receivedBytes == -1)
					errnoCheck("recv()");
				else if (receivedBytes == 0)
				{
					std::cout << "client connection lost :(" << std::endl;
					close(clientSocket);
					break;
				}
				buffer[receivedBytes - 1] = '\0';
				std::string		pass(buffer);
				if (pass != this->password)
				{
					tries--;
					passwordRequest =  "Invalid Password, please try harder this time (" + std::to_string(tries) + ") remaining: ";
					
				}
				else
					passwordPending = false;
			}
			else
			{
				receivedBytes = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				if (receivedBytes == -1)
					errnoCheck("recv()");
				else if (receivedBytes == 0)
				{
					std::cout << "client connection lost :(" << std::endl;
					close(clientSocket);
					passwordPending = true;
					break;
				}
				//for first time check password (give 3 tries ...)
				buffer[receivedBytes - 1] = '\0';
				if (send(clientSocket, buffer, strlen(buffer), 0) == -1)
					errnoCheck("send()");
			}
		}
		close (clientSocket);
		close (servSocket);
	}
}

bool	Server::checkPassword(int fd)
{
	;
}

User	*Server::createUser(int fd)
{
	;
}