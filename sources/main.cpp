
#include "Server.hpp"


/*-- 
* a valid port number range from 1 to 65535
--*/
bool    parse_args(char **s)
{
    if (!validPort(s[1]))
    {
        errorMsg(PORT);
        return (false);
    }
    if (!validPass(s[2]))
    {
        errorMsg(PASSWORD);
        return (false);
    }
    return (true);
}

int main(int ac, char **av, char **env)
{
    if (ac != 3)
    {
        errorMsg(USAGE);
        return (EXIT_FAILURE);
    }
    if (!parse_args(av))
        return (EXIT_FAILURE);
    std::string pass(av[2]);
    Server banana(atoi(av[1]), pass);
    banana.createConnection();
}