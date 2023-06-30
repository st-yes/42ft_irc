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
    while (i != strlen(p)){
        if (p[i] == '#' && i != 0)
            return true;
        if (p[i] == ':' || p[i] == ',' || p[i] == 7)
            return true;
        i++;
    }
    return false;
}

std::vector<std::string>    Server::divideAndConquer(std::string s, User *userX){
    std::vector<std::string> store;
    char                     *p = NULL;
    char                     *hold = const_cast<char*>(s.c_str());
    std::string              *params;
    std::cout << "hnaaa ::::" << s << std::endl;
    std::cout << "what ::: " << hold << std::endl;
    if(s.empty())
        return store;
    if (s.find(',') != std::string::npos){
        p = strtok(hold, ",");
        while (p != NULL){
            std::cout << "le bananeur :" << p << std::endl;
            if (!syntaxCheck(p))
                store.push_back(p);
            else{
                params = allocateForParams(1);
                params[0] = userX->getNickForReply() + " " + std::string (p);
                this->sendReply(userX->sendFd, this->serverName, ERR_NOSUCHCHANNEL, params);
                delete [] params;
                return store;
            }
            for (int i = 0; i != store.size(); i++)
                std::cout << store[i] << std::endl;
            p = strtok(NULL, ",");
        }
    }
    else{
        if (!syntaxCheck(hold)){
            store.push_back(s);
        }
        else{
            params = allocateForParams(1);
            params[0] = userX->getNickForReply() + " " + std::string (p);
            this->sendReply(userX->sendFd, this->serverName, ERR_NOSUCHCHANNEL, params);
            delete [] params;
            return store;
        }
    }
    return store;
}

void    Server::handleCmdJoin(std::string *s, User *userX, int paramNum){
    std::unordered_map<std::string, std::string>  channels;
    bool                                check;
    std::vector<std::string>            chanStore;
    std::vector<std::string>            passStore;
    std::string                         *params;

    params = allocateForParams(1);
    if (paramNum > 3){
        params[0] =  userX->getNickForReply();
        sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS , params);
    }
    chanStore = divideAndConquer(s[1], userX);
    if (!s[2].empty())
        passStore = divideAndConquer(s[2], userX);
    else
        passStore.push_back("");
    if (chanStore.size() == 0 || passStore.size() == 0 ||chanStore.size() != passStore.size()){
        params[0] =  userX->getNickForReply();
        sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS , params);
    }
    for (int i = 0; i != chanStore.size(); i++)
        channels.insert(std::make_pair(chanStore[i], passStore[i]));
    delete [] params;
    this->JoinFunc(channels, userX);
    return;
}

// void    Server::handleCmdJoin(std::string* str, User* Userx, int paramNumber)
// {
//     int i = 0;
//     int f;
//     std::map<std::string, std::string> Channels;
//     std::vector<std::string> Channls;
//     std::vector<std::string> Pass;
//     if (str[1][0] == '0')
//     {
//         str[1] = Userx->currentChannel->channelName;
//         str[2] = Userx->nextChannel->channelName;
//         this->handleCmdPart(str, Userx, paramNumber);
//         return ;
//     }
//     while(++i < 3)
//     {
//         switch (i)
//         {
//             case 1:{
//                 f = str[i].find("#", 1);  
//                 while (f != std::string::npos){
//                     std::string s = str[i].substr(0, f - 1);
//                     if (s.find("#") == std::string::npos || s.find(":") != std::string::npos
//                         || s.find(",")!= std::string::npos || s.find(7) != std::string::npos){
//                             std::string *param = allocateForParams(1);
//                             param[0] = Userx->getNickForReply() + " " + std::string (str[i]);
//                             this->sendReply(Userx->sendFd, this->serverName, ERR_NOSUCHCHANNEL, param);
//                             delete [] param;
//                             return;
//                         }
//                     Channls.push_back(s);
//                     str[i] = str[i].substr(f);
//                     f = str[i].find("#", 1);
//                 }
//                 Channls.push_back(str[i].substr(0));
//                 break ; 
//             }
//             case 2:{
//                 f = str[i].find(",");
//                 while (f != std::string::npos){ // 
//                     Pass.push_back(str[i].substr(0, f));
//                     str[i] = str[i].substr(f + 1);
//                     f = str[i].find(",");
//                 }
//                 Pass.push_back(str[i].substr(0));
//             }
//         }
//     }
//     int q = 0;
//     while(q < Channls.size())
//     {
        
//     }
//     if (Channls.size() >= 2)
//     {
//         int k;
//         bool flag;
//         if (Userx->nextChannel->channelName != "#Lobby!" && Channls[0] != "#Lobby!" && Channls[1] != "#Lobby!"){
//             k = 0;
//             flag = true;
//         }
//         else
//             k = 2;
//         std::string *param = allocateForParams(1);
//         while (k != Channls.size()){
//             param[0] =  Userx->getNickForReply() + " " + std::string (Channls[k]);
//             sendReply(Userx->sendFd, this->serverName, ERR_TOOMANYCHANNELS , param);
//             k++;
//         }
//         if (Pass.size() == 1 )
//             Pass.push_back("");
//         Channels.insert(std::pair<std::string, std::string>(Channls[1], Pass[1]));
//         delete [] param;
//         if (Pass.size() < 2)
//             Pass.push_back("");
//         if (!flag)
//             Channels.insert(std::pair<std::string, std::string>(Channls[1], Pass[1]));
//     }
//     Channels.insert(std::pair<std::string, std::string>(Channls[0], Pass[0]));
//     this->JoinFunc(Channels, Userx);
// }

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
    this->sendGenericReply(Userx, "JOIN", nw, "");
    this->sendReplyNames(Userx, RPL_NAMREPLY, nw);
}

void    Server::JoinFunc(std::unordered_map<std::string, std::string>   tmp, User* Userx){
    int channelCreation = 0;
    /* !!!! if the channel is private he need invitation!!!! === */
    std::unordered_map<std::string, std::string>::iterator p = tmp.begin();
    std::cout << tmp.size() << std::endl;
    while(p != tmp.end())
    {
        // if (Userx->nextChannel->channelName != "#Lobby!" && p->first != "#Lobby!"){
        //     std::string *param = allocateForParams(1);
        //     param[0] = Userx->getNickForReply() + " " + " " + p->first;
        //     this->sendReply(Userx->sendFd, this->serverName, ERR_TOOMANYCHANNELS, param);
        //     delete [] param;
        // return;
        // }
        Channel   *Chanl = this->channelFinder(p->first);
        if (Chanl == NULL && channelCreation < 2){
            this->newChannel(p, Userx);
            channelCreation++;
        }
        else if (Chanl == NULL && channelCreation >= 2){
            std::string *param = allocateForParams(1);
            param[0] =  Userx->getNickForReply() + " " + std::string (p->first);            
            sendReply(Userx->sendFd, this->serverName, ERR_TOOMANYCHANNELS , param);
            delete [] param;
        }
        else{
            if (Chanl->inviteMode && !channelFinder(Chanl->channelName, Userx->invitedChannels)
                && findUserinChan(Userx->sendFd, Chanl->channelOps) == -1){
                std::string *param = allocateForParams(1);
                param[0] = Userx->getNickForReply() + " " + Chanl->channelName;
                this->sendReply(Userx->sendFd, this->serverName, ERR_INVITEONLYCHAN, param);
                delete [] param;
            }
            else if (Chanl->limitMode && Chanl->channelMembers.size() >= Chanl->getChanLimit()){
                std::string *param = allocateForParams(1);
                param[0] = Userx->getNickForReply() + " " + Chanl->channelName;
                this->sendReply(Userx->sendFd, this->serverName, ERR_CHANNELISFULL, param);
                delete [] param;
            }
            else if (Chanl->keyMode && Chanl->GetThekey() != p->second){
                std::string *param = allocateForParams(1);
                param[0] = Userx->getNickForReply() + " " + Chanl->channelName;
                this->sendReply(Userx->sendFd, this->serverName, ERR_BADCHANNELKEY, param);
                delete [] param;
            }
            else{
                Chanl->channelMembers.push_back(Userx);
                Userx->joinedChannels.push_back(Chanl);
                this->sendGenericReply(Userx, "JOIN", Chanl, "");
                this->sendReplyNames(Userx, RPL_NAMREPLY, Chanl);
                std::vector<User *>::iterator it; // test for chanops and ircGod
                for (it = Chanl->channelOps.begin(); it != Chanl->channelOps.end(); it++){
                    std::cout << "CHANELOPS :" << Chanl->channelOps.begin() - it << (*it)->getNick() << std::endl;
                }
                for (it = Chanl->channelMembers.begin(); it != Chanl->channelMembers.end(); it++){
                    std::cout << "USERS :" << Chanl->channelMembers.begin() - it << (*it)->getNick() << std::endl;
                }
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
    std::vector<std::string >::iterator it;
    

   this->ParsePart(channel, str, reason);
    int i = 0;
    
    for(it = channel.begin(); it != channel.end();){
        Channel   *Chanl = this->channelFinder(it->c_str());
        if (Chanl == NULL)// please recheck this : Done
                return;
        int j = Chanl->findUserinChannel(Userx->sendFd); 
        if (j != -1){
            User *tmp = Chanl->channelMembers[j];
            Chanl->channelMembers.erase(Chanl->channelMembers.begin() + j);
            int it = this->channelFinder2(Chanl->channelName, Userx->joinedChannels);
            Userx->joinedChannels.erase(Userx->joinedChannels.begin() + it);
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
            }
            this->sendGenericReply(Userx, "PART", Chanl, "");
        }
        else{
            std::string *param = allocateForParams(1);
            param[0] = Userx->getNickForReply() + " " + Chanl->channelName;
            this->sendReply(Userx->sendFd, this->serverName, ERR_USERNOTINCHANNEL, param);
            delete [] param;
        }
        it++;
    }

}

// void    Server::handleCmdKick(std::string *params, User *userX, int paramNumber){

//     int it;
//     Channel *tmp = this->channelFinder(params[1]);
//     User *ustmp = this->findUserinServ(params[2]);
//     if (tmp == NULL)
//         {std::cout << "Channel not exist\n";return;} 
//     if (ustmp == NULL)
//         {std::cout << "this user doesn't exist\n"; return ;}
//     if ((this->findUserinChan(userX->sendFd, tmp->channelOps)) == -1)//check the sender is a chanop;
//         {std::cout << "this user is not chnops\n";return;}
//     if ((this->findUserinChan(ustmp->sendFd, tmp->channelOps)) != -1)//check if the reciver is not a chanop
//         {std::cout << "this user is a chnops\n";return;}
//     if (it = (this->findUserinChan(ustmp->sendFd, tmp->channelMembers)) == -1)
//         {std::cout << "this user is not in the channel \n";return ;}
//     // std::cout<< "size of channel before :" << tmp->channelMembers.size() << std::endl ;
//     tmp->channelMembers.erase(tmp->channelMembers.begin() + it);
//     // std::cout<< "size of channel after :" << tmp->channelMembers.size() << std::endl ;
//     ustmp->currentChannel = ustmp->nextChannel;
//     ustmp->nextChannel = this->servChannels[0];
//     this->sendGenericReply(userX, "KICK", tmp, "");
//     this->sendGenericReply(ustmp, "part", tmp, "");
//     /*reason for kick is the param[3]!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
// }

// void    Server::handleCmdInvite(std::string *params, User *userX, int paramNumber){
//     std::cout << "cmd :" << params[0] << std::endl;
//     std::cout<< "channel name :" << params[1] << std::endl;
//     std::cout<< "user who we need to remove : " << params[2] << std::endl;
//     std::cout<< "reason for is kick" << params[3] << std::endl;

//     User *ustmp = this->findUserinServ(params[2]); 
//     if (ustmp == NULL)
//         {std::cout << "this user doesn't exist\n"; return ;}
//     Channel *tmp = this->channelFinder(params[1]);//check if channel exist if not we call join func
//     if (tmp == NULL)
//     {
//         std::string *param = allocateForParams(2);
//         param[0] = "join";
//         param[1] = params[2];
//         this->handleCmdJoin(param, userX, paramNumber);
//         //invite msj for the new channel ;
//     }
//     else{
//         if (tmp->inviteMode == true)
//         {
//             if ((this->findUserinChan(userX->sendFd, tmp->channelOps)) == -1)//check the sender is a chanop;
//                 {std::cout << "this user is not chnops\n";return;}
//             //invite msj here ; is chanop 
//         }
//         else
//            ; // invite msj : the channel in public.
//     }

      
// }

void    Server::handleCmdInvite(std::string *params, User *userX, int paramNumber){
    std::string *param = allocateForParams(1);
    if (paramNumber > 3){
        param[0] =  userX->getNickForReply() + " :Wrong Syntax";
        sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS , param);
        delete [] param;
        return;
    }
    Channel *chanX = channelFinder(params[2]);
    User    *receiver = this->findUserinServ(params[1]);
    if (!chanX || !receiver){
        // err channel or user do not exist!
        param[0] =  userX->getNickForReply() + " :User or Channel do not exist";
        sendReply(userX->sendFd, this->serverName, ERR_NOSUCHNICK, param);
        delete [] param;
        return;
    }
    int     chanopCheck = this->findUserinChan(userX->sendFd, chanX->channelOps); // if sender is a ChanOp
    int     checker_ = this->findUserinChan(userX->sendFd, chanX->channelMembers); // if receiver is already a member of the channel
    if (chanopCheck == -1){
        // Sender is not a ChanOp
        param[0] =  userX->getNickForReply() + " " + chanX->channelName + " :You are not Chanop!";
        sendReply(userX->sendFd, this->serverName, ERR_CHANOPRIVSNEEDED, param);
        delete [] param;
        return; 
    }
    if (checker_ == -1){
        // err the inviter is not in the channel
        param[0] =  userX->getNickForReply() + " " + chanX->channelName + " :User not in Channel!";
        sendReply(userX->sendFd, this->serverName, ERR_USERONCHANNEL, param);
        delete [] param;
        return;
    }
    //if (chanX->inviteMode){
        receiver->invitedChannels.push_back(chanX);
        param[0] = userX->getNickForReply() + " has invited you!";
        sendReply(receiver->sendFd, this->serverName, RPL_INVITING, param);
        // send message
    //}
    //else{
        // err the channel is already public
        // params[0] =  userX->getNickForReply();
        // sendReply(userX->sendFd, this->serverName, ERR_USERONCHANNEL, params);
    //}
    delete [] param;
    return ;
}

void    Server::handleCmdKick(std::string *params, User *userX, int paramNumber){
    std::string *param = allocateForParams(1);
    if (paramNumber > 3){
        // err too many arguments
        param[0] =  userX->getNickForReply() + " :Wrong Syntax";
        sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS , param);
        delete [] param;
        return;
    }
    Channel *chanX = channelFinder(params[1]);
    User    *receiver = this->findUserinServ(params[2]);
    if (!chanX || !receiver){
        // err Channel or user do not exist
        param[0] =  userX->getNickForReply() + " :User or Channel do not exist";
        sendReply(userX->sendFd, this->serverName, ERR_NOSUCHNICK, param);
        delete [] param;
        return;
    }
    int     chanopCheck = this->findUserinChan(userX->sendFd, chanX->channelOps); // if sender is a ChanOp
    std::cout <<  "||||--->" << chanopCheck << std::endl;
    int     checker_ = this->findUserinChan(receiver->sendFd, chanX->channelMembers); // if receiver is already a member of the channel
    if (chanopCheck == -1){
        // Sender is not a ChanOp
        param[0] =  userX->getNickForReply() + " " + chanX->channelName + " :You are not Chanop!";
        sendReply(userX->sendFd, this->serverName, ERR_CHANOPRIVSNEEDED, param);
        delete [] param;
        return;
    }
    if (checker_ == -1){
        // err the inviter is not in the channel
        param[0] =  userX->getNickForReply() + " :User is not in the channel!";
        sendReply(userX->sendFd, this->serverName, ERR_USERONCHANNEL, param);
        delete [] param;
        return;
    }
    else{
        int i;
        for (i = 0; i != receiver->joinedChannels.size(); i++){
            if (receiver->joinedChannels[i]->channelName == chanX->channelName)
                break;
        }
        receiver->joinedChannels.erase(receiver->joinedChannels.begin() + i);
        chanX->channelMembers.erase(chanX->channelMembers.begin() + checker_);
        // send kick message here
        sendGenericReply(receiver, "KICK" ,chanX, "");
        sendGenericReply(receiver,"PART", chanX, "");
    }
}