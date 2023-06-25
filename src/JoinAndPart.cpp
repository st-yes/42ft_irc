# include "Ft_irc.hpp"

std::string Channel::GetThekey(){
    return this->channelKey;
}

int CheckChannelIFexist(std::vector<Channel*> tmp, std::map<std::string, std::string>::iterator   tmp3){
    
    std::vector<Channel *>::iterator it = tmp.begin();
    int i = 0;
    while(i < tmp.size())
    {
        if (tmp[i]->channelName == tmp3->first)
        {
            return 0;
        }
        i++;
    }
    return i;
}
void    Server::handleCmdJoin(std::string* str, User* Userx, int paramNumber)
{
    int i = 0;
    int f;
    std::map<std::string, std::string> Channels;
    std::vector<std::string> Channls;
    std::vector<std::string> Pass;
    if (str[1][0] == '0')
    {
        str[1] = Userx->currentChannel->channelName;
        str[2] = Userx->nextChannel->channelName;
        this->handleCmdPart(str, Userx, paramNumber);
        return ;
    }
    while(++i < 3)
    {
        switch (i)
        {
            case 1:{
                f = str[i].find(",");
                while (f != std::string::npos){
                    Channls.push_back(str[i].substr(0, f));
                    str[i] = str[i].substr(f + 1);
                    f = str[i].find(",");
                }
                Channls.push_back(str[i].substr(0));
                break ; 
            }
            case 2:{
                f = str[i].find(",");
                while (f != std::string::npos){
                    Pass.push_back(str[i].substr(0, f));
                    
                    str[i] = str[i].substr(f + 1);
                    f = str[i].find(",");
                }
                Pass.push_back(str[i].substr(0));
            }
        }
    }
    if (Channls.size() > 2)
    {
        std::string *param = allocateForParams(1);
        param[0] = Userx->getNickForReply();
        sendReply(Userx->sendFd, this->serverName, ERR_TOOMANYCHANNELS , param);
        return ;
    }
    i = 0;
    if (Channls.size() == 2){
        Channels.insert(std::pair<std::string, std::string>(Channls[1], Pass[1]));
    }
    Channels.insert(std::pair<std::string, std::string>(Channls[0], Pass[0]));
    this->JoinFunc(Channels, Userx);
}

void    Server::JoinFunc(std::map<std::string, std::string>   tmp, User* Userx){

    /* !!!! if the channel is private he need invitation!!!! === */
    std::map<std::string, std::string>::iterator p = tmp.begin();
    std::cout << tmp.size() << std::endl;
    while(p != tmp.end())
    {
        // if (this->users.find(i)->second->nextChannel->channelName != "#Lobby!" && p->first != "#Lobby!") |STEPH|
        //     {std::cout<< this->users.find(i)->second->nextChannel->channelName << "   ur limit channels to join is 2\n";return;}
    
        Channel   *Chanl = this->channelFinder(p->first);
        Channel   *nw;
        if (Chanl == NULL)
        {
            nw = new Channel(p->first, "", p->second);
            nw->channelMembers.push_back(Userx);
            Userx->nextChannel = Userx->currentChannel;
            Userx->currentChannel = nw;
            this->servChannels.push_back(nw);
            nw->channelOps.push_back(Userx);
        }
        else{
            if (Chanl->inviteMode && !channelFinder(Chanl->channelName, Userx->invitedChannels)){
                std::string *param = allocateForParams(1);
                param[0] = Userx->getNickForReply() + " " + Chanl->channelName;
                this->sendReply(Userx->sendFd, this->serverName, ERR_INVITEONLYCHAN, param);
                delete [] param;
                return;
            }
            if (Chanl->limitMode && Chanl->channelMembers.size() >= Chanl->getChanLimit()){
                std::string *param = allocateForParams(1);
                param[0] = Userx->getNickForReply() + " " + Chanl->channelName;
                this->sendReply(Userx->sendFd, this->serverName, ERR_CHANNELISFULL, param);
                delete [] param;
                return;
            }
            if (Chanl->GetThekey() != p->second) // || or the user need invite if the channel is private
            {
                std::string *param = allocateForParams(1);
                param[0] = Userx->getNickForReply() + " " + Chanl->channelName;
                this->sendReply(Userx->sendFd, this->serverName, ERR_BADCHANNELKEY, param);
                delete [] param;
                return;
            }
            Chanl->channelMembers.push_back(Userx);
            Userx->nextChannel = Userx->currentChannel;
            Userx->currentChannel = Chanl;
        }
        this->sendGenericReply(Userx, "JOIN", nw);
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
    tmp2 = str[1].substr(str[1].find(",") + 1);
    channel.push_back(tmp);
    channel.push_back(tmp2);
}

void    Server::handleCmdPart(std::string* str, User* Userx, int paramNumber){

    std::string channel2;
    std::string reason;
    std::vector<std::string >channel;
    std::vector<std::string >::iterator it;

   this->ParsePart(channel, str, reason);
    int i = 0;

    for(it = channel.begin(); it != channel.end();it++){
        Channel   *Chanl = this->channelFinder(it->c_str());
        if (Chanl == NULL){ // please recheck this
            std::cout << "the channel not exist\n";
                return;
        }
        int j = Chanl->findUserinChannel(Userx->sendFd); 
        if (j != -1){
            User *tmp = Chanl->channelMembers[j];
            Chanl->channelMembers.erase(Chanl->channelMembers.begin() + j);
            if (tmp->currentChannel->channelName == it->c_str()){
                    tmp->currentChannel = tmp->nextChannel;
                tmp->nextChannel = this->servChannels[0]; // this-> serverchannels[0] == default == lobby;
            }
            else{
                if (this->servChannels[0])
                    tmp->nextChannel = this->servChannels[0];
            }
            if(Chanl->channelMembers.size() == 0)
            {
                for (std::vector<Channel*>::iterator it = this->servChannels.begin(); it != this->servChannels.end(); it++){
                    if ((*it)->channelName == Chanl->channelName)
                    {
                        int f = it - this->servChannels.begin();
                        this->servChannels.erase(this->servChannels.begin() + f);
                        break ;
                    }
                }
            }
            this->sendGenericReply(Userx, "PART", Chanl);
        }
        else{
            std::string *param = allocateForParams(1);
            param[0] = Userx->getNickForReply() + " " + Chanl->channelName;
            this->sendReply(Userx->sendFd, this->serverName, ERR_USERNOTINCHANNEL, param);
            delete [] param;
        }
    }

}
