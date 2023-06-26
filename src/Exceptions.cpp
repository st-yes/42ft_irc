#include "../includes/Ft_irc.hpp"

const char* Server::errorNbArguments::what() const throw()
{
    return "Error: invalid number of arguments"; 
}
const char* Server::errorPort::what() const throw()
{
    return "Error: invalid Port number";
}
const char* Server::errorPass::what() const throw()
{
    return "Error: invalid password";
}

const char* Server::errorErrno::what() const throw()
{
    const char* errorDescription = std::strerror(errno);
    std::string errorMessage = "Error code: " + std::to_string(errno) + " " + "Description: " + errorDescription;

    char* errorMessagePtr = new char[errorMessage.size() + 1];

    std::strcpy(errorMessagePtr, errorMessage.c_str());

    return errorMessagePtr;
}