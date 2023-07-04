#include "../includes/Ft_irc.hpp"

std::string Channel::GetThekey(){
    return this->channelKey;
}

int CheckChannelIFexist(std::vector<Channel*> tmp, std::map<std::string, std::string>::iterator   tmp3){
    
    std::vector<Channel *>::iterator it = tmp.begin();
    int i = 0;
    while(i < tmp.size())
    {
        if (tmp[i]->channelName == tmp3->first)
            return 0;
        i++;
    }
    return i;
}

bool    Server::syntaxCheck(char *p){
    if(!p)
        return true;
    int i = 0;
    if (p[0] != '#')
            return true;
    while (i != strlen(p)){ 
        if ((p[i] == '#' && i != 0))
            return true;
        if (p[i] == ':' || p[i] == ',')//7?
            return true;
        i++;
    }
    return false;
}

std::vector<std::string>    Server::divideAndConquer(std::string s, User *userX){
    std::vector<std::string> store;
    char                     *p = NULL;
    char                     *hold = const_cast<char*>(s.c_str());
    std::vector<User *>      send;
    send.push_back(userX);

    if(s.empty())
        return store;
    if (s.find(',') != std::string::npos){
        p = strtok(hold, ",");
        while (p != NULL){
            if (!syntaxCheck(p))
                store.push_back(p);
            else{
                this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHCHANNEL, "Channel does not Exist : " + std::string(p)), send);
                return store;
            }
            // for (int i = 0; i != store.size(); i++)
            //     std::cout << store[i] << std::endl;
            p = strtok(NULL, ",");
        }
    }
    else{
        if (!syntaxCheck(hold)){
            store.push_back(s);
        }
        else{
            this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHCHANNEL, "Channel does not Exist :" + std::string(hold)), send);
            return store;
        }
    }
    return store;
}

std::vector<std::string>    Server::divideAndConquer2(std::string s, User *userX){
    std::vector<std::string> store;
    char                     *p = NULL;
    char                     *hold = const_cast<char*>(s.c_str());
    std::vector<User *>      send;
    send.push_back(userX);

    if(s.empty())
        return store;
    if (s.find(',') != std::string::npos){
        p = strtok(hold, ",");
        while (p != NULL){
            store.push_back(p);
            p = strtok(NULL, ",");
        }
    }
    else
        store.push_back(s);
    return store;
}

void    Server::handleCmdJoin(std::string *s, User *userX, int paramNum){
    std::unordered_map<std::string, std::string>  channels;
    bool                                check;
    std::vector<std::string>            chanStore;
    std::vector<std::string>            passStore;
    std::vector<User *>                 send;
    send.push_back(userX);

    if (paramNum > 3){
        this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "Syntax Error!"), send);
    }
    chanStore = divideAndConquer(s[1], userX);
    if (!s[2].empty())
        passStore = divideAndConquer2(s[2], userX);
    else
        passStore.push_back("");
    if (chanStore.empty() || (!s[2].empty() && chanStore.size() != passStore.size())){
        this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "Syntax Error!"), send);
        return;
    }
    for (int i = 0; i != chanStore.size(); i++)
    {
        channels.insert(std::make_pair(chanStore[i], passStore[i]));
    }
    this->JoinFunc(channels, userX);
    return;
}

void    Server::newChannel(std::unordered_map<std::string, std::string>::iterator p, User* Userx){

    Channel   *nw;
    nw = new Channel(p->first, "", p->second);
    nw->channelMembers.push_back(Userx);
    Userx->joinedChannels.push_back(nw);
    this->servChannels.push_back(nw);
    this->addNewChanops(Userx, nw);
    if (p->second != ""){
        nw->keyMode = true;
        nw->setTheKey(p->second);
    }
    std::vector<User *>send;
    send.push_back(Userx);
    this->sendHermes(this->sendGenericCode(Userx, nw, "JOIN", ""), send);
    this->sendHermes(this->sendNumericCode(Userx, nw, RPL_NAMREPLY, ""), send);
    this->sendHermes(this->sendNumericCode(Userx, nw, RPL_ENDOFNAMES, ""), send);

}

void    Server::JoinFunc(std::unordered_map<std::string, std::string>   tmp, User* Userx){
    int channelCreation = 0;
    std::vector<User *>send;
    send.push_back(Userx);
    std::unordered_map<std::string, std::string>::iterator p = tmp.begin();
    while(p != tmp.end())
    {
        Channel   *Chanl = this->channelFinder(p->first);
        if (Chanl == NULL && Userx->createdChannels < 2){
            this->newChannel(p, Userx);
            Userx->createdChannels++;
        }
        else if (Chanl == NULL && channelCreation >= 2){
            this->sendHermes(this->sendNumericCode(Userx, Chanl, ERR_TOOMANYCHANNELS, ""), send);
        }
        else{
            if (Chanl->inviteMode && !channelFinder(Chanl->channelName, Userx->invitedChannels)
                && findUserinChan(Userx->sendFd, Chanl->channelOps) == -1){
                this->sendHermes(this->sendNumericCode(Userx, Chanl, ERR_INVITEONLYCHAN, ""), send);
            }
            else if (Chanl->limitMode && Chanl->channelMembers.size() >= Chanl->getChanLimit()){
                this->sendHermes(this->sendNumericCode(Userx, Chanl, ERR_BADCHANNELKEY, ""), send);
            }
            else if (Chanl->keyMode && Chanl->GetThekey() != p->second){
                this->sendHermes(this->sendNumericCode(Userx, Chanl, ERR_BADCHANNELKEY, ""), send);
            }
            else{
                Chanl->channelMembers.push_back(Userx);
                Userx->joinedChannels.push_back(Chanl);
                this->sendHermes(this->sendGenericCode(Userx, Chanl, "JOIN", ""), Chanl->channelMembers);
                this->sendHermes(this->sendNumericCode(Userx, Chanl, RPL_TOPIC, Chanl->channelTopic), send);
                this->sendHermes(this->sendNumericCode(Userx, Chanl, RPL_NAMREPLY, ""), send);
                this->sendHermes(this->sendNumericCode(Userx, Chanl, RPL_ENDOFNAMES, ""), send);
            }
        }
        p++;
    }
    return ;
}

void    Server::ParsePart(std::vector<std::string> &channel,std::string* str, std::string& reason){

    std::vector<std::string>::iterator it;
    std::string tmp;
    std::string tmp2;
    if (str[2] != "")
        reason = str[2];
    tmp = str[1].substr(0, str[1].find(","));
    str[1] = str[1].substr(tmp.size());
    tmp2 = str[1].substr(str[1].find(',') + 1);
    channel.push_back(tmp);
    if (tmp2 != ""){
        channel.push_back(tmp2);
    }
}

void    Server::handleCmdPart(std::string* str, User* Userx, int paramNumber){

    std::string channel2;
    std::string reason;
    std::vector<std::string >channel;
    std::vector<User *>      send;
    std::vector<std::string >::iterator it;
    

   this->ParsePart(channel, str, reason);
    int i = 0;
    
    for(it = channel.begin(); it != channel.end();){
        Channel   *Chanl = this->channelFinder(it->c_str());
        if (Chanl == NULL)
                return;
        int j = Chanl->findUserinChannel(Userx->sendFd); 
        if (j != -1){
            User *tmp = Chanl->channelMembers[j];
            int it = this->channelFinder2(Chanl->channelName, Userx->joinedChannels);
            Userx->joinedChannels.erase(Userx->joinedChannels.begin() + it);
            this->sendHermes(this->sendGenericCode(Userx, Chanl, "PART", ""), Chanl->channelMembers);
            Chanl->channelMembers.erase(Chanl->channelMembers.begin() + j);
            if(Chanl->channelMembers.size() == 0 && Chanl->channelName != "#Lobby!")
            {
                for (std::vector<Channel*>::iterator it = this->servChannels.begin(); it != this->servChannels.end(); it++){
                    if ((*it)->channelName == Chanl->channelName)
                    {
                        int f = it - this->servChannels.begin();
                        this->servChannels.erase(this->servChannels.begin() + f);
                        break ;
                    }
                }
                delete Chanl;
            }
        }
        else{
            send.push_back(Userx);
            this->sendHermes(this->sendNumericCode(Userx, Chanl, ERR_USERNOTINCHANNEL, ""), send);
        }
        it++;
    }

}

void    Server::handleCmdInvite(std::string *params, User *userX, int paramNumber){
    std::vector<User *> send;
    send.push_back(userX);
    if (paramNumber > 3){
        this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "Wrong Syntax!"), send);
        return;
    }
    Channel *chanX = channelFinder(params[2]);
    User    *receiver = this->findUserinServ(params[1]);
    if (!chanX || !receiver){
        if (!chanX)
            this->sendHermes(this->sendNumericCode(userX, chanX, ERR_NOSUCHCHANNEL, "Nick is not valid"), send);
        else if (!receiver)
            this->sendHermes(this->sendNumericCode(userX, chanX, ERR_NOSUCHNICK, "Channel does not exist"), send);
        return;
    }
    int     chanopCheck = this->findUserinChan(userX->sendFd, chanX->channelOps); 
    int     checker_ = this->findUserinChan(receiver->sendFd, chanX->channelMembers);
    if (chanopCheck == -1){
        this->sendHermes(this->sendNumericCode(userX, chanX, ERR_CHANOPRIVSNEEDED, "You are not a Chanop!"), send);
        return; 
    }
    if (checker_ != -1){
        this->sendHermes(this->sendNumericCode(userX, chanX, ERR_USERONCHANNEL, "The invited user is already in the channel"), send);
        return;
    }
        receiver->invitedChannels.push_back(chanX);
        std::vector<User *> send2;
        send2.push_back(receiver);
        this->sendHermes(this->sendNumericCode(receiver, chanX, RPL_INVITING, "You have been invited!"), send2);
    return ;
}

void    Server::handleCmdKick(std::string *params, User *userX, int paramNumber){
    std::vector<User *> send;
    send.push_back(userX);
    if (paramNumber > 3){
        this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "Wrong Syntax"), send);
        return;
    }
    Channel *chanX = channelFinder(params[1]);
    User    *receiver = this->findUserinServ(params[2]);
    if (!chanX || !receiver){
        if (!chanX)
            this->sendHermes(this->sendNumericCode(userX, chanX, ERR_NOSUCHCHANNEL, "Nick is not valid"), send);
        else if (!receiver)
            this->sendHermes(this->sendNumericCode(userX, chanX, ERR_NOSUCHNICK, "Channel does not exist"), send);
        return;
    }
    int     chanopCheck = this->findUserinChan(userX->sendFd, chanX->channelOps);
    int     checker_ = this->findUserinChan(receiver->sendFd, chanX->channelMembers);
    if (chanopCheck == -1){
        this->sendHermes(this->sendNumericCode(userX, chanX, ERR_CHANOPRIVSNEEDED, "You are not a Chanop!"), send);
        return;
    }
    if (checker_ == -1){
        this->sendHermes(this->sendNumericCode(userX, chanX, ERR_USERNOTINCHANNEL, "The user is not in the channel!"), send);
        return;
    }
    else{
        int i;
        for (i = 0; i != receiver->joinedChannels.size(); i++){
            if (receiver->joinedChannels[i]->channelName == chanX->channelName)
                break;
        }
        receiver->joinedChannels.erase(receiver->joinedChannels.begin() + i);
        std::vector<User *> send2;
        send2.push_back(receiver);
        this->sendHermes(this->sendGenericCode(receiver, chanX, "PART", ""), send2);
        this->sendHermes(this->sendGenericCode(userX, chanX, "KICK", (receiver->getNickForReply() + " :" + params[3])), chanX->channelMembers);
        chanX->channelMembers.erase(chanX->channelMembers.begin() + checker_);
    }
}