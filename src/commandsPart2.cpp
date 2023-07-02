# include "../includes/Ft_irc.hpp"

void    Server::handleCmdOper(std::string   *params, User *userX, int paramNumber){
    std::vector <User *> send;
    send.push_back(userX);
    if (paramNumber != 2){
            this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "Syntax Error!"), send);
    }
    else{
        std::map<std::string, std::string>::iterator it;
        it = this->gods.find(params[1]);
        if (it != this->gods.end() && it->second == params[2]){
            ircGod *elite = new ircGod(userX, it->first);
            for(int k = 0; k != userX->joinedChannels.size(); k++){
                Channel *p = userX->joinedChannels[k];
                int i = p->findUserinChannel(userX->sendFd);
                if (i != -1)
                    p->channelMembers.erase(p->channelMembers.begin() + i);
                p->channelMembers.push_back(elite);
            }
            this->users.erase(userX->sendFd);
            this->users.insert(std::make_pair(elite->sendFd, elite));
            for (int k = 0; k != this->servChannels.size(); k++)
                this->servChannels[k]->channelOps.push_back(elite);
            delete userX;
            elite->setGodPower(this);
            std::vector<User*>send2;
            send2.push_back(userX);
            this->sendHermes(this->sendNumericCode(elite, NULL, RPL_YOUREOPER, "GODLY POWERS! HERE WE GO!"), send2);
        }
        else{
            this->sendHermes(this->sendNumericCode(userX, NULL, ERR_PASSWDMISMATCH, "Invalid Credentials"), send);
        }
    }
}

std::string Server::getChannelPriv(std::string s){
    std::string privs = "+";
    Channel *Channel = this->channelFinder(s);
    if(!Channel)
        return "N/A";
    if (Channel->inviteMode){
        privs += "i";
    }
    if (Channel->topicProtectMode){
        privs += "t";
    }
    if (Channel->limitMode){
        privs += "l";
    }
    if (Channel->keyMode){
        privs += "k";
    }
    return privs;
}

void    Server::handleCmdMode(std::string *param, User *userX, int paramNumber){
    std::vector<User *> send;
    send.push_back(userX);
    if (paramNumber == 1 && param[1].find("#") != std::string::npos){
        this->sendHermes(this->sendNumericCode(userX, NULL, RPL_CHANNELMODEIS, std::string(getChannelPriv(param[1]))), send);
        return;
    }
    if (paramNumber > 5){
        this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "not enough parameters!"), send);
        return;
    }
    int check = checkStringToken(param, paramNumber);
    if (check > 2){
        if (paramNumber == 2 && param[1] == userX->getNickForReply()){
            this->sendHermes(this->sendNumericCode(userX, NULL, RPL_UMODEIS, "+i"), send);
            return;
        }
        else{
            this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "Syntax Error"), send);
            return;
        }
    }
    else{
        switch (check){
            case 1:{
                this->sendHermes(this->sendNumericCode(userX, NULL, ERR_USERNOTINCHANNEL, "Channel not specified"), send);
                return;
            }
            case 2:{
                Channel *currentCh = userX->channelFinderinUser(param[1]);
                if (currentCh){
                    this->handleCmdModeOpt(param, userX, currentCh);
                }
                else{
                    Channel *find = this->channelFinder(param[1]);
                    if (!find){
                        this->sendHermes(this->sendNumericCode(userX, NULL, ERR_USERNOTINCHANNEL, "Channel does not exist"), send);
                        return;
                    }
                    this->handleCmdModeOpt(param, userX, find);
                } 
            }
        }
    }
}

void    Server::handleCmdModeOpt(std::string *params, User *userX, Channel *Ch){
    int paramIndex = 2;
    std::map<char, std::string> options;
    std::vector<User *>         send;
    send.push_back(userX);
    int mode = -1;
    if (params[2].c_str()[0] == '+')
        mode = 1;
    else if (params[2].c_str()[0] == '-')
        mode = 0;
    for (int i = 1; i != params[2].length(); i++){
        if (params[2].c_str()[i] == 'k' || params[2].c_str()[i] == 'l' || params[2].c_str()[i] == 'o'){
            paramIndex++;
            if (params[paramIndex] == ""){
                this->sendHermes(this->sendNumericCode(userX, Ch, ERR_NEEDMOREPARAMS, "Not enough parameters for the option specified"), send);
                return;
            }
            options.insert(std::make_pair(params[2].c_str()[i], params[paramIndex]));
        }
        else if(params[2].c_str()[i] == 't' || params[2].c_str()[i] == 'i')
            options.insert(std::make_pair(params[2].c_str()[i], params[1]));
        else{
            this->sendHermes(this->sendNumericCode(userX, Ch, ERR_NEEDMOREPARAMS, "The options specified are not correct! \" itkol \""), send);
            return;
        }
    }
    if (mode != -1)
        this->handleCmdModeOpt(Ch, userX, options, mode);
    else
        return;
}

void    Server::handleCmdModeOpt(Channel *chan, User *userX, std::map<char, std::string> opt, int mode){
    bool    check;
    for(std::map<char, std::string>::iterator it = opt.begin(); it != opt.end(); it++){
        switch(it->first){
            case 'i' :{
                this->handleCmdModeOptI(userX, chan, mode);
                break;
            }
            case 't':{
                this->handleCmdModeOptT(userX, chan, mode);
                break;
            }
            case 'o':{
                this->handleCmdModeOptO(userX, it->second, chan, mode);
                break;
            }
            case 'k':{
                this->handleCmdModeOptK(userX, it->second, chan, mode);
                break;
            }
            case 'l':{
                this->handleCmdModeOptL(userX, it->second, chan, mode);
                break;
            }
            default :{
                std::vector<User *> send;
                send.push_back(userX);
                this->sendHermes(this->sendNumericCode(userX, chan, ERR_UNKNOWNMODE, "The mode is unknown"), send);
                break;
            }
        }
    }
    return;
}
void    Server::handleCmdModeOptI(User *userX, Channel *chan, int mode){
    std::vector<User*>  send;
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    send.push_back(userX);
    if (!God && this->findUserinChan(userX->sendFd, chan->channelOps) == -1){
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_CHANOPRIVSNEEDED, "You are not a chanOp!"), send);
        return;
    }
    if (mode == 0 && !chan->inviteMode){
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_INVITEONLYCHAN, "Invite Only is already Off!"), send);
    }
    else if (mode == 1 && chan->inviteMode){
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_INVITEONLYCHAN, "Invite only is already On!"), send);
    }
    else if (mode == 0 && chan->inviteMode){
         chan->inviteMode = false;
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The invite only Mode is now off"), send);
        this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "-i"), chan->channelMembers);
    }
    else if (mode == 1 && !chan->inviteMode){
         chan->inviteMode = true;
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The invite only Mode is now on"), send);
        this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "+i"), chan->channelMembers);

    }
    return ;
}
void    Server::handleCmdModeOptT(User *userX, Channel *chan, int mode){
        ircGod  *God = dynamic_cast<ircGod*>(userX);
        std::vector<User *> send;
    if (this->findUserinChan(userX->sendFd, chan->channelOps) == -1 && !God){
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_CHANOPRIVSNEEDED, "You are not a chanOp"), send);
        return;
    }
    if (mode == 0 && !chan->topicProtectMode){
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, "The topic protection Mode is already off"), send);
    }
    else if (mode == 1 && chan->topicProtectMode){
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, "The topic protection Mode is already off"), send);
    }
    else if (mode == 0 && chan->topicProtectMode){
        chan->topicProtectMode = false;
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The topic protection Mode is now off"), send);
        this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "-t"), chan->channelMembers);
    }
    else if (mode == 1 && !chan->topicProtectMode){
        chan->topicProtectMode = true;     
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The topic protection Mode is now on"), send);
        this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "+t"), chan->channelMembers);
    }
    return ;
}
void    Server::handleCmdModeOptO(User *userX, std::string s, Channel *chan, int mode){
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    User    *player = findUserinServ(s);
    std::vector<User*> send;
    send.push_back(userX);
    if(!God){
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_NOPRIVILEGES, "you dont have the powers to do that"), send);
    }
    else if (God && (!player || this->findUserinChan(player->sendFd, chan->channelMembers) == -1)){
       this->sendHermes(this->sendNumericCode(userX, chan, ERR_USERNOTINCHANNEL, "User specified is not in the channel"), send);        
    }
    else{
        if (mode == 1 && this->findUserinChan(player->sendFd, chan->channelOps) == -1){
            chan->channelOps.push_back(player);
            this->sendHermes(this->sendNumericCode(userX, chan, RPL_ENDOFBANLIST, "you are now a chanOp"), send);
        }
        else if (mode == 0 && this->findUserinChan(player->sendFd, chan->channelOps) != -1){
           this->sendHermes(this->sendNumericCode(userX, chan, RPL_ENDOFBANLIST, "you are no longer a chanOp"), send);
        }
        else if (mode == 1 && this->findUserinChan(player->sendFd, chan->channelOps) != 1){
           this->sendHermes(this->sendNumericCode(userX, chan, RPL_ENDOFBANLIST, "you are already a chanOp"), send);
        
        }
        else if (mode == 0 && this->findUserinChan(player->sendFd, chan->channelOps) == -1){        
           this->sendHermes(this->sendNumericCode(userX, chan, RPL_ENDOFBANLIST, "you are already a chanOp"), send);
        
        }
    }
    return;
}
void    Server::handleCmdModeOptK(User *userX, std::string s, Channel *chan, int mode){
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    std::vector<User*> send;
   send.push_back(userX);
    if (!God && this->findUserinChan(userX->sendFd, chan->channelOps) == -1){
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_CHANOPRIVSNEEDED, "you are mot a chanOp"), send);
        return;
    }
    if (chan->keyMode){
        switch (mode){
            case 0 :{
                chan->keyMode = false;
                this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODEIS, "Channel no longer has a key"), chan->channelMembers);
                this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "-k"), send);
                break;
            }
            case 1 :{
                chan->keyMode = true;
                chan->setTheKey(s);
                this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "Channel has a new password"), chan->channelMembers);
                break;
            }
            default :
                break; 
        }
    }
    else if (!chan->keyMode){
        switch (mode){
            case 0 :{
                this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The channel already is open!"), chan->channelMembers);
            
                break;
            }
            case 1 :{
                chan->keyMode = true;
                chan->setTheKey(s);
                this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The channel has new key"), send);
                this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "+k"), send);
                break;
            }
            default :
                break;
        }
    }
}
void    Server::handleCmdModeOptL(User *userX, std::string s, Channel *chan, int mode){
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    std::vector<User *>      send;
    send.push_back(userX);
    if (!God && this->findUserinChan(userX->sendFd, chan->channelOps) == -1){
       this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "you are not a chanOp"), send);
        return;
    }
    bool    check = checkNums(s);
    if (check){
        size_t limit = stoi(s);
        if (chan->limitMode){
            switch (mode){
                case 0 :{
                    chan->limitMode = false;
                    this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "channel is no longer limited"), chan->channelMembers);
                    this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "-l"), send);
                    break;
                }
                case 1 :{
                    if (limit == 0)
                        chan->limitMode = false;
                    else
                        chan->limitMode = true;
                    chan->setTheLimit(limit);
                    this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "channel has new limit "), chan->channelMembers);
                    break;
                }
                default :
                    break; 
            }
        }
        else if (!chan->limitMode){
            switch (mode){
                case 0 :{
                    this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "channel already has no limit"), send);
                    break;
                }
                case 1 :{
                    if (limit == 0)
                        chan->limitMode = false;
                    else
                        chan->limitMode = true;
                    chan->setTheLimit(limit);
                    this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "has a new limit "), send);
                    this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "+l"), send);
                    break;
                }
                default :
                    break;
            }
        }
    }
    else{
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_NEEDMOREPARAMS, "Your parameter is non numeric"), send);
    }
    return;
}

