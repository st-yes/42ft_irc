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
        str[1] = this->users[Userx->sendFd]->currentChannel->channelName;
        str[2] = this->users[Userx->sendFd]->nextChannel->channelName;
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
        std::string *tr = allocateForParams(1);
        tr[0] = ": To many Channels";
        sendReply(Userx->sendFd, "blackiie", ERR_TOOMANYCHANNELS , tr);
        std::string msj = "ERROR :TOO MANY CHANNELS\r\n";
        if (send(Userx->sendFd, msj.c_str(), msj.length(), 0) == -1)
		    throw errorErrno();
        return ;
    }
    i = 0;
    if (Channls.size() == 2){
        Channels.insert(std::pair<std::string, std::string>(Channls[1], Pass[1]));
    }
    Channels.insert(std::pair<std::string, std::string>(Channls[0], Pass[0]));
    this->JoinFunc(Channels, Userx->sendFd);
}

void    Server::JoinFunc(std::map<std::string, std::string>   tmp, int i){

    /* !!!! if the channel is private he need invitation!!!! === */
    std::map<std::string, std::string>::iterator p = tmp.begin();
    std::cout << tmp.size() << std::endl;
    while(p != tmp.end())
    {
        // if (this->users.find(i)->second->nextChannel->channelName != "#Lobby!" && p->first != "#Lobby!")
        //     {std::cout<< this->users.find(i)->second->nextChannel->channelName << "   ur limit channels to join is 2\n";return;}
        int it = CheckChannelIFexist(this->servChannels, p);
        if (i == 0){
                this->servChannels[it]->channelMembers.push_back(this->users.find(i)->second);
                this->users.find(i)->second->nextChannel = this->users.find(i)->second->currentChannel;
                this->users.find(i)->second->currentChannel = this->servChannels[it];

        }
        else{
                Channel *nw = new Channel(p->first, "", p->second);
                nw->channelMembers.push_back(this->users.find(i)->second);
                this->users.find(i)->second->nextChannel = this->users.find(i)->second->currentChannel;
                this->users.find(i)->second->currentChannel = nw;
                this->servChannels.push_back(nw);
            }
        p++;
    }
    std::string reply = ":styes JOIN frkouss \r\n";
    if (send(i, reply.c_str(), reply.length(), 0) == -1)
		throw errorErrno();
    std::cout << reply << std::endl;
};

void    Server::ParsePart(std::string& channel, std::string& channel2, std::string* str, std::string& reason){

    if (str[2] != "")
        reason = str[2];
    channel = str[1].substr(0, str[1].find(","));
    str[1] = str[1].substr(str[1].find(",") + 1);
    if (str[1] != channel && str[1][0] != '#'){
        str[1] = '#' + str[1];
        channel2 = str[1];
    }
    if (channel[0] != '#')
        channel = '#' + channel;
}

void    Server::handleCmdPart(std::string* str, User* Userx, int paramNumber){

    std::string channel2;
    std::string channel;
    std::string reason;

   ParsePart(channel, channel2, str, reason);
    // if (channel.empty() == 1)
    //     std::cout << "yaaak akhuya hasan \n"; return ;
    int i = 0;


    while(i < this->servChannels.size())
    {
        if (channel == "Lobby!")
            {std::cout << "u can't get out from ur default channel\n";return;}
        std::cout << "channel name " << this->servChannels[i]->channelName << std::endl;
        if (this->servChannels[i]->channelName == channel)
        {
            // check if the user in this channel;
            int j = 0;
            while(j < this->servChannels[i]->channelMembers.size())
            {
                if (this->servChannels[i]->channelMembers[j]->sendFd == Userx->sendFd)
                {
                    User *tmp = this->servChannels[i]->channelMembers[j];
                    this->servChannels[i]->channelMembers.erase(this->servChannels[i]->channelMembers.begin() + j);

                        // std::cout << "========++===========" <<this->users.find(Userx->sendFd)->second->currentChannel->channelName << std::endl;
	                    // std::cout << "========++==========" <<this->users.find(Userx->sendFd)->second->nextChannel->channelName << std::endl;
                    if (this->users.find(Userx->sendFd)->second->currentChannel->channelName == channel){
                         this->users.find(Userx->sendFd)->second->currentChannel = this->users.find(Userx->sendFd)->second->nextChannel;
                        this->users.find(Userx->sendFd)->second->nextChannel = this->servChannels[0];
                        // std::cout << "===++++++++++==++====" <<this->users.find(Userx->sendFd)->second->currentChannel->channelName << std::endl;
	                    // std::cout << "==++++++++++++==++====" <<this->users.find(Userx->sendFd)->second->nextChannel->channelName << std::endl;
                    }
                    else{
                        std::cout << "hola senior \n";
                        this->users.find(Userx->sendFd)->second->nextChannel = this->servChannels[0];
                    }//
                    std::string reply = ":styes PART " + this->servChannels[i]->channelName + "\r\n";
                        if (send(Userx->sendFd, reply.c_str(), reply.length(), 0) == -1)
		                    throw errorErrno();
                    std::cout << "|" << this->servChannels[i]->channelName << "|" << std::endl; 
                    std::cout << "here" + reply << std::endl;
                    // delete tmp;
                    if (channel2 != "")
                    {
                        channel = channel2;
                        i = 0;
                        break;
                    }
                    else{
                        return ;}
                }
                j++;
                if (j == this->servChannels[i]->channelMembers.size())
                    {std::cout << "u r not member in this channel\n";return;}
            }
        }
        i++;
        if (i == this->servChannels.size())
        {
            std::cout << "the channel not exist\n";
            return;
        }
    }

}

