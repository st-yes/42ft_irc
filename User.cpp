#include "User.hpp"

User::User(){
}

User::User(std::string name, std::string nick) : User_Name(name), User_Nick(nick){
    std::cout << this->User_Nick << "has been created" << std::endl;
}

User::~User(){
    std::cout << this->User_Nick << "has been killed!" << std::endl;
}

User::User(User const &p){
}

User const & User::operator=(User const &p){
    *this = p;
    return *this;
}