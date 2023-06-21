#include "../includes/ircGod.hpp"

ircGod::ircGod(){}
ircGod::ircGod(User const *s, std::string name) : User(*s), godName(name){
    std::cout << "A god has been made! " << std::endl;
}
ircGod::ircGod(ircGod const &s) : User(s){
    this->godName = s.godName;
    this->servAccess = s.servAccess;
}
ircGod::~ircGod(){
    std::cout << "Hayhayhay Siba hadi!" << std::endl; 
}
ircGod const &ircGod::operator=(ircGod const &s){
    *this = s;
    return *this;
}