// #include "Request.hpp"

// Request::Request(){}
// Request::Request(std::string s){
//     char        *token = strtok(s.c_str(), " ");
//     std::string Array[] = {"PING", "PASS", "USER", "JOIN", "PRVMSG", "KICK", "INVITE", "TOPIC", "MODE", "PART"};

//     while (token){

//         token = strtok(NULL, " ");
//     }

// }
// Request::Request(Request const &s){
//     *this = s;
// }
// Request::~Request(){
//     std::cout << "No more Requests to treat!" << std::endl;
// }
// Request const &Request::operator=(Request const &s){
//     return *this;
// }