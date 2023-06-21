# include "Ft_irc.hpp"

std::string Channel::GetThekey(){
    return this->channelKey;
}

int CheckChannelIFexist(std::vector<Channel*> tmp, std::map<std::string, std::string>::iterator   tmp3){
    
    std::vector<Channel *>::iterator it = tmp.begin();
    // std::map<std::string, std::string>::iterator t = tmp3.begin();
    int i = 0;
    

    while(i < tmp.size())
    {
        if (tmp[i]->channelName == tmp3->first)
            return 0;
        i++;
    }
    // std::cout << tmp[i]->channelName << std::endl;
    // std::cout << t->first << std::endl;
    return i;
}

void    Server::ParseJoin(std::string* str, User* Userx, int paramNumber)
{
    int i = 1;
    int f;
    std::map<std::string, std::string> Channels;
    std::vector<std::string> Channls;
    std::vector<std::string> Pass;
    while(i < 3)
    {
        if (i == 1)
        {
            f = str[i].find(",");
            while (f != std::string::npos){
                Channls.push_back(str[i].substr(0, f));
                str[i] = str[i].substr(f + 1);
                f = str[i].find(",");
            }
            Channls.push_back(str[i].substr(0));
        }
        else{
            f = str[i].find(",");
            while (f != std::string::npos){
                Pass.push_back(str[i].substr(0, f));
                str[i] = str[i].substr(f + 1);
                f = str[i].find(",");
            }
            Pass.push_back(str[i].substr(0));
        }
        i++;
    }
    if (Channls.size() > 2)
        return ;
    i = 0;
    Channels.insert(std::pair<std::string, std::string>(Channls[0], Pass[0]));
    if (Channls.size() == 2)
        Channels.insert(std::pair<std::string, std::string>(Channls[1], Pass[1]));
    std::cout << Channels.size() << std::endl;
    this->JoinFunc(Channels, Userx->sendFd);
    
}

void    Server::JoinFunc(std::map<std::string, std::string>   tmp, int i){

    /* !!!! if the channel is private he need invitation!!!! === */
    /*if the string == 0; the user should leave all the channels
     and will be in the lobby.*/
    // add the channels to currentchannels and nextchannel.
    std::map<std::string, std::string>::iterator p = tmp.begin();
    while(p != tmp.end())
    {
        // std::cout << p->first  << " === " << p->second << std::endl;
        int it = CheckChannelIFexist(this->servChannels, p);
        if (it == 0) // channel exist
        {
            // if (this->servChannels[i]->Get_thekey() != "test") /*check the password*/
            //     return ; 
            this->servChannels[it]->channelMembers.push_back(this->users.find(i)->second);
            this->users.find(i)->second->nextChannel = this->users.find(i)->second->currentChannel;
            this->users.find(i)->second->currentChannel = this->servChannels[it];
            // if (++p == tmp.end())
            // {
            //     this->servChannels[it]->channelMembers.push_back(this->users.find(i)->second);
            //     this->users.find(i)->second->nextChannel = this->users.find(i)->second->currentChannel;
            //     this->users.find(i)->second->currentChannel = this->servChannels[it];
            // }
        }
        else{ // new channel 

            // std::cout << i << std::endl;
            Channel *nw = new Channel(p->first, "", p->second);
            // std::map<int, User*>::iterator q = this->users.find(i);
            nw->channelMembers.push_back(this->users.find(i)->second);
            this->users.find(i)->second->nextChannel = this->users.find(i)->second->currentChannel;
            this->users.find(i)->second->currentChannel = nw;
            this->servChannels.push_back(nw);
            // if (++p == tmp.end())
            // {
            //     Channel *nw = new Channel(p->first, "", p->second);
            //     nw->channelMembers.push_back(this->users.find(this->pollers[i].fd)->second);
            //     this->users.find(this->pollers[i].fd)->second->nextChannel = this->users.find(this->pollers[i].fd)->second->currentChannel;
            //     this->users.find(this->pollers[i].fd)->second->currentChannel = nw;
            //     this->servChannels.push_back(nw);   
            // }
        }
        std::cout << p->first  << " = before == " << p->second << std::endl;
        p++;
    }
    std::cout << "Current channel " << this->users.find(i)->second->currentChannel->channelName << std::endl;
    std::cout << "next channel " << this->users.find(i)->second->nextChannel->channelName << std::endl;
};