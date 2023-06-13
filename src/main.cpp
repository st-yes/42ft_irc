#include "../includes/everyThing.hpp"

int main(int ac, char **av){
    if (ac != 3)
    {
        errorMsg(USAGE);
        return (EXIT_FAILURE);
    }
    if (!parse_args(av))
        return (EXIT_FAILURE);
    std::string pass(av[2]);
    Server      run(atoi(av[1]), pass);
    run.createConnection();
    return (0);
}