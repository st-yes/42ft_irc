# include "../includes/Ft_irc.hpp"

void    Server::handleCmdOper(std::string   *params, User *userX, int paramNumber){
    if (paramNumber != 2){
        std::string *paramsRep;
        if (paramNumber > 2){
            paramsRep = allocateForParams(1);
            paramsRep[0] = ":too many parameters";
            sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS , paramsRep);
            delete[] paramsRep;
        }
        else if (paramNumber < 2){
            paramsRep = allocateForParams(2);
            paramsRep[0] = "OPER";
            paramsRep[1] = ":need more parameters";
            sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, paramsRep);
            delete[] paramsRep;
        }
    }
    else{
        std::map<std::string, std::string>::iterator it;
        it = this->gods.find(params[1]);
        if (it != this->gods.end() && it->second == params[2]){
            ircGod *elite = new ircGod(userX, it->first);
            Channel *p = userX->currentChannel;
            if (userX->currentChannel == NULL){
                std::cout << "There has been a problem, User not assigned to a channel!" << std::endl;
                this->lostConnection(userX);
                return;
            }
            int i = p->findUserinChannel(userX->sendFd);
            if (i != -1)
                p->channelMembers.erase(p->channelMembers.begin() + i);
            this->users.erase(userX->sendFd);
            this->users.insert(std::make_pair(elite->sendFd, elite));
            p->channelMembers.push_back(elite);
            for (int k = 0; k != this->servChannels.size(); k++)
                this->servChannels[k]->channelOps.push_back(elite);
            delete userX;
            elite->setGodPower(this);
            std::string *paramsRep;
            paramsRep = allocateForParams(2);
            paramsRep[0] = "OPER";
            paramsRep[1] = ":A God among the living";
            sendReply(userX->sendFd, this->serverName, RPL_YOUREOPER, paramsRep);
            delete [] paramsRep;
        }
        else{
            std::string *paramsRep;
            paramsRep = allocateForParams(2);
            paramsRep[0] = "OPER";
            paramsRep[1] = ":invalid credentials";
            sendReply(userX->sendFd, this->serverName, ERR_PASSWDMISMATCH, paramsRep);
            delete [] paramsRep;
        }
    }
}

void    Server::handleCmdMode(std::string *param, User *userX, int paramNumber){
    std::string *params;
    if (paramNumber > 5){
        params = allocateForParams(1);
        params[0] = "Not enough parameter for the options specified!";
        this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
        delete [] params;
        return;
    }
    int check = checkStringToken(param, paramNumber);
    if (check > 2){
        //Error of +- or too many Channels;
        params = allocateForParams(1);
        params[0] = "Error in the syntax used!";
        this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
        delete [] params;
        return;
    }
    else{
        switch (check){
            case 1:{
                // Error, no channel given
                params = allocateForParams(1);
                params[0] = "No Channel has been specified!";
                this->sendReply(userX->sendFd, this->serverName, ERR_USERNOTINCHANNEL, params);
                delete [] params;
                return;
            }
            case 2:{
                if (userX->currentChannel->channelName == param[1]){ // check is empty param[1] mohim hiya li fiha l mochkil
                    // treat as current Channel
                    this->handleCmdModeOpt(param, userX, userX->currentChannel);
                }
                else{
                    // treat as external Channel
                    Channel *find = this->channelFinder(param[1]);
                    if (!find){
                        //Error Channel doesnt exist
                        params = allocateForParams(1);
                        params[0] = "Channel specified does not exist!";
                        this->sendReply(userX->sendFd, this->serverName, ERR_USERNOTINCHANNEL, params);
                        delete [] params;
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
    std::string *param;
    int mode = -1;
    if (params[2].c_str()[0] == '+')
        mode = 1;
    else if (params[2].c_str()[0] == '-')
        mode = 0;
    for (int i = 1; i != params[2].length(); i++){
        if (params[2].c_str()[i] == 'k' || params[2].c_str()[i] == 'l' || params[2].c_str()[i] == 'o'){
            paramIndex++;
            if (params[paramIndex] == ""){
                // Error not enough parameters
                params = allocateForParams(1);
                params[0] = "Not enough parameter for the options specified!";
                this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
                delete [] params;
                return;
            }
            options.insert(std::make_pair(params[2].c_str()[i], params[paramIndex]));
        }
        else if(params[2].c_str()[i] == 't' || params[2].c_str()[i] == 'i')
            options.insert(std::make_pair(params[2].c_str()[i], params[1]));
        else{
            // Error mode option not correct
            param = allocateForParams(1);
            param[0] = "The Options specified are not correct! \" itkol \"";
            this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
            delete [] param;
            return;
        }
    }
    if (mode != -1)
        this->handleCmdModeOpt(Ch, userX, options, mode);
    else
        return;
}

void    Server::handleCmdModeOpt(Channel *chan, User *userX, std::map<char, std::string> opt, int mode){
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
            default :
                break;
        }
    }
    return;
}
void    Server::handleCmdModeOptI(User *userX, Channel *chan, int mode){
    std::string *params;
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    if (!God && this->findUserinChan(userX->sendFd, chan->channelOps) == -1){
        //Error User is not a chanop
        params = allocateForParams(1);
        params[0] = "MODE : You are not a Chanop!";
        this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPPRIVNEEDED, params);
        delete [] params;
        return;
    }
    if (mode == 0 && !chan->inviteMode){
        params = allocateForParams(1);
        params[0] = "MODE : The Invite only Mode is already off!";
        this->sendReply(userX->sendFd, this->serverName, ERR_INVITEONLYCHAN, params);
    }
    else if (mode == 1 && chan->inviteMode){
        params = allocateForParams(1);
        params[0] = "MODE : The Invite only Mode is already On!";
        this->sendReply(userX->sendFd, this->serverName, ERR_INVITEONLYCHAN, params);
    }
    else if (mode == 0 && chan->inviteMode){
        chan->inviteMode = false;
        params = allocateForParams(1);
        params[0] = "MODE : The Invite only Mode is now Off!";
        this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
    }
    else if (mode == 1 && !chan->inviteMode){
        chan->inviteMode = true;
        params = allocateForParams(1);
        params[0] = "MODE : The Invite only Mode is now On!";
        this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
    }
    delete [] params;
    return ;
}
void    Server::handleCmdModeOptT(User *userX, Channel *chan, int mode){
        std::string *params;
        ircGod  *God = dynamic_cast<ircGod*>(userX);
    if (this->findUserinChan(userX->sendFd, chan->channelOps) == -1 && !God){
        //Error User is not a chanop
        params = allocateForParams(1);
        params[0] = "MODE : You are not a Chanop!";
        this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPPRIVNEEDED, params);
        delete [] params;
        return;
    }
    if (mode == 0 && !chan->topicProtectMode){
        params = allocateForParams(1);
        params[0] = "MODE : The Topic Protection Mode is already off!";
        this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, params);
    }
    else if (mode == 1 && chan->topicProtectMode){
        params = allocateForParams(1);
        params[0] = "MODE : The Topic Protection Mode is already On!";
        this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, params);
    }
    else if (mode == 0 && chan->topicProtectMode){
        chan->inviteMode = false;
        params = allocateForParams(1);
        params[0] = "MODE : The Topic Protection Mode is now Off!";
        this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
    }
    else if (mode == 1 && !chan->topicProtectMode){
        chan->inviteMode = true;
        params = allocateForParams(1);
        params[0] = "MODE : The Topic Protection Mode is now On!";
        this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
    }
    delete [] params;
    return ;
}
void    Server::handleCmdModeOptO(User *userX, std::string s, Channel *chan, int mode){
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    User    *player = findUserinServ(s);
    std::string *params;
    std::string *params2;
    if(!God){
        // Error you dont have IRCGod powers.
        params = allocateForParams(1);
        params[0] = "MODE : You are not a God!";
        this->sendReply(userX->sendFd, this->serverName, ERR_NOPRIVILEGES, params);
    }
    else if (God && (!player || this->findUserinChan(player->sendFd, chan->channelMembers) == -1)){
        // Error User doesnt exist
        params = allocateForParams(1);
        params[0] = "MODE : User does not exist in channel: " + chan->channelName;
        this->sendReply(userX->sendFd, this->serverName, ERR_USERNOTINCHANNEL, params);
    }
    else{
        if (mode == 1 && this->findUserinChan(player->sendFd, chan->channelOps) == -1){
            chan->channelOps.push_back(player);
            params = allocateForParams(1);
            params2 = allocateForParams(1);
            params[0] = "MODE :" + player->getNick() + " is now a Chanop of " + chan->channelName;
            params2[0] = "MODE : You are now a Chanop of " + chan->channelName; 
            this->sendReply(God->sendFd, this->serverName, RPL_ENDOFBANLIST, params);
            this->sendReply(player->sendFd, this->serverName, RPL_ENDOFBANLIST, params2);
            delete [] params2;
        }
        else if (mode == 0 && this->findUserinChan(player->sendFd, chan->channelOps) != -1){
            params = allocateForParams(1);
            params2 = allocateForParams(1);
            params[0] = "MODE :" + player->getNick() + " is no longer a Chanop of " + chan->channelName;
            params2[0] = "MODE : You are no longer a Chanop of " + chan->channelName;
            this->sendReply(God->sendFd, this->serverName, RPL_ENDOFBANLIST, params);
            this->sendReply(player->sendFd, this->serverName, RPL_ENDOFBANLIST, params2);
            delete [] params2;
        }
        else if (mode == 1 && this->findUserinChan(player->sendFd, chan->channelOps) != 1){
            params = allocateForParams(1);
            params[0] = "MODE : " + player->getNick() + " is already a ChanOp!";
            this->sendReply(player->sendFd, this->serverName, RPL_ENDOFBANLIST, params); // User is already a channel Operator}
        }
        else if (mode == 0 && this->findUserinChan(player->sendFd, chan->channelOps) == -1){
            params = allocateForParams(1);
            params[0] = "MODE : " + player->getNick() + " is already not a ChanOp!";
            this->sendReply(player->sendFd, this->serverName, RPL_ENDOFBANLIST, params); // User is not a channel Operator
        }
    }
    delete [] params;
    return;
}
void    Server::handleCmdModeOptK(User *userX, std::string s, Channel *chan, int mode){
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    std::string *params;
    if (!God && this->findUserinChan(userX->sendFd, chan->channelOps) == -1){
        //Error User is not a chanop
        params = allocateForParams(1);
        params[0] = "MODE : You are not a Chanop!";
        this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPPRIVNEEDED, params);
        delete [] params;
        return;
    }
    if (chan->keyMode){
        switch (mode){
            case 0 :{
                chan->keyMode = false;
                params = allocateForParams(1);
                params[0] = "MODE :" + chan->channelName + " no longer has a password!";
                this->sendReply(userX->sendFd, this->serverName, RPL_NOTOPIC, params);
                delete [] params;
                break;
            }
            case 1 :{
                chan->keyMode = true;
                chan->setTheKey(s);
                params = allocateForParams(1);
                params[0] = "MODE :" + chan->channelName + "has a new password!";
                this->sendReply(userX->sendFd, this->serverName, RPL_INVITING, params);
                delete [] params;
                break;
            }
            default :
                break; 
        }
    }
    else if (!chan->keyMode){
        switch (mode){
            case 0 :{
                params = allocateForParams(1);
                params[0] = "MODE : " + chan->channelName + " already has no password";
                this->sendReply(userX->sendFd, this->serverName, RPL_NOTOPIC, params);
                delete [] params;
                break;
            }
            case 1 :{
                chan->keyMode = true;
                chan->setTheKey(s);
                params = allocateForParams(1);
                params[0] = "MODE : " +chan->channelName + "has a new password!";
                this->sendReply(userX->sendFd, this->serverName, RPL_INVITING, params);
                delete [] params;
                break;
            }
            default :
                break;
        }
    }
}
void    Server::handleCmdModeOptL(User *userX, std::string s, Channel *chan, int mode){
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    std::string *params;
    if (!God && this->findUserinChan(userX->sendFd, chan->channelOps) == -1){
        //Error User is not a chanop
        params = allocateForParams(1);
        params[0] = "MODE : You are not a Chanop!";
        this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPPRIVNEEDED, params);
        delete [] params;
        return;
    }
    bool    check = checkNums(s);
    if (check){
        size_t limit = stoi(s);
        if (chan->limitMode){
            switch (mode){
                case 0 :{
                    chan->limitMode = false; // error there s no limit 
                    params = allocateForParams(1);
                    params[0] = "MODE :" + chan->channelName + " is no longer limited!";
                    this->sendReply(userX->sendFd, this->serverName, RPL_NOTOPIC, params);
                    delete [] params;
                    break;
                }
                case 1 :{
                    if (limit == 0)
                        chan->limitMode = false;
                    else
                        chan->limitMode = true;
                    chan->setTheLimit(limit);
                    params = allocateForParams(1);
                    params[0] = "MODE :" + chan->channelName + "has a new limit!"; 
                    this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
                    delete [] params;
                    break;
                }
                default :
                    break; 
            }
        }
        else if (!chan->keyMode){
            switch (mode){
                case 0 :{
                    params = allocateForParams(1);
                    params[0] = "MODE :" + chan->channelName + " already has no limit";
                    this->sendReply(userX->sendFd, this->serverName, RPL_NOTOPIC, params);
                    delete [] params;
                    break;
                }
                case 1 :{
                    if (limit == 0)
                        chan->keyMode = false;
                    else
                        chan->keyMode = true;
                    chan->setTheLimit(limit);
                    params = allocateForParams(1);
                    params[0] = "MODE" + chan->channelName + "has a new limit!";
                    this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
                    delete [] params;
                    break;
                }
                default :
                    break;
            }
        }
    }
    else{
        //Error here for non numeric argument
        params = allocateForParams(1);
        params[0] = " MODE : Your argument is non numeric!"; 
        this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
        delete [] params;
    }
    return;
}

Channel   *Server::channelFinder(std::string s){
    if(this->servChannels.empty())
        return NULL;
    for (std::vector<Channel*>::iterator it = this->servChannels.begin(); it != this->servChannels.end(); it++){
        if ((*it)->channelName == s)
            return (*it);
    }
    return NULL;
}

