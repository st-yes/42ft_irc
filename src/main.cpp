#include "Ft_irc.hpp"

int main(int ac, char **av)
{
    try
    {
        Server  server(ac, av);
        server.createConnection();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}