#include "Server.hpp"

bool    parse_args(char **s){
    int i = 0;
    while (i < 4){
        if (!isnumber(s[1][i])){
            std::cerr << "Please make sure the arguments are correct, Both the Port and PassWord have to be positive integers." << std::endl;
            std::cout << "->" <<  s[1][i]  << "<-" << std::endl;
            std::cout << i << std::endl;
            return false;}
        i++;
    }
    return true;
}

int main(int ac, char **av, char **env){
    if (ac != 3 || !parse_args(av))
    {
        std::cerr << "Bad Arguments!" << std::endl;
        return 1;
    }
    Server banana(atoi(av[1]), atoi(av[2]));
    banana.Create_Connection();
}