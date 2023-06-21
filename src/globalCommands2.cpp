# include "../includes/Ft_irc.hpp"

void    Server::handleCmdOper(std::string   *params, User *userX, int paramNumber){
    int fd = this->findClientFd(userX);
    if (paramNumber != 2){
        std::string *paramsRep;
        if (paramNumber > 2){
            paramsRep = allocateForParams(1);
            paramsRep[0] = ":too many parameters";
            sendReply(fd, this->serverName, "X" , paramsRep);
            delete[] paramsRep;
        }
        else if (paramNumber < 2){
            paramsRep = allocateForParams(2);
            paramsRep[0] = "OPER";
            paramsRep[1] = ":need more parameters";
            sendReply(fd, this->serverName, ERR_NEEDMOREPARAMS, paramsRep);
            delete[] paramsRep;
        }
    }
    else{
        std::map<std::string, std::string>::iterator it;
        it = this->gods.find(params[1]);
        if (it != this->gods.end() && it->second == params[2]){
            ircGod *elite = new ircGod(userX, it->first);
            Channel *p = userX->currentChannel;
            if (p == NULL){
                std::cout << "hok" << std::endl;
                return;
            }
            int i = p->findUserinChannel(fd);
            if (i != -1)
                p->channelMembers.erase(p->channelMembers.begin() + i);
            this->users.erase(fd);
            this->users.insert(std::make_pair(fd, elite));
            p->channelMembers.push_back(elite);
            for (int k = 0; k != this->servChannels.size(); k++){
                this->servChannels[k]->channelOps.push_back(elite);
            }
            delete userX;
            elite->servAccess = this;
            std::string *paramsRep;
            paramsRep = allocateForParams(2);
            paramsRep[0] = "OPER";
            paramsRep[1] = ":A God among the living";
            sendReply(fd, this->serverName, RPL_YOUREOPER, paramsRep);
            delete [] paramsRep;
        }
        else{
            std::string *paramsRep;
            paramsRep = allocateForParams(2);
            paramsRep[0] = "OPER";
            paramsRep[1] = ":invalid credentials";
            sendReply(fd, this->serverName, ERR_PASSWDMISMATCH, paramsRep);
            delete [] paramsRep;
        }
    }
}

