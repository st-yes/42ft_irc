#include "Ft_irc.hpp"

void listenOnAddress(uint32_t ipAddress, uint16_t port) {
    int sockfd, newsockfd;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    // Bind socket to IP address and port
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(ipAddress); // Network byte order IP address
    serverAddress.sin_port = htons(port); // Network byte order port

    if (bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(sockfd);
        return;
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) == -1) {
        std::cerr << "Failed to listen for connections" << std::endl;
        close(sockfd);
        return;
    }

    std::cout << "Server listening on IP: " << inet_ntoa(serverAddress.sin_addr)
              << ", Port: " << ntohs(serverAddress.sin_port) << std::endl;

    // Accept incoming connection
    clientAddressLength = sizeof(clientAddress);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (newsockfd == -1) {
        std::cerr << "Failed to accept connection" << std::endl;
        close(sockfd);
        return;
    }

    std::cout << "Client connected: " << inet_ntoa(clientAddress.sin_addr) << std::endl;

    // Handle the connection
    // ...

    // Close the sockets
    close(newsockfd);
    close(sockfd);
}

void Server::dcc(std::string	str)
{

	std::ifstream	iss(str);
	std::string		token;
	uint32_t		ip;
	bool			first = true;
	while (std::getline(iss, token, ' '))
	{
		if (isdigit(token[0]) && first)
			ip = std::atoll(token);
	}
}