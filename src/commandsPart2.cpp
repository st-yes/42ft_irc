# include "../includes/Ft_irc.hpp"

void    Server::handleCmdOper(std::string   *params, User *userX, int paramNumber){
    std::vector <User *> send;
    send.push_back(userX);
    if (paramNumber != 2){
        //std::string *paramsRep;
        //if (paramNumber > 2){
            // paramsRep = allocateForParams(1);
            // paramsRep[0] = ":too many parameters";
            // sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS , paramsRep);
            // delete[] paramsRep;
            this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "Syntax Error!"), send);
    }
    else{
        std::map<std::string, std::string>::iterator it;
        it = this->gods.find(params[1]);
        if (it != this->gods.end() && it->second == params[2]){
            ircGod *elite = new ircGod(userX, it->first);
            if (userX->joinedChannels.empty()){
                std::cout << "There has been a problem, User not assigned to a channel!" << std::endl;
                this->lostConnection(userX);
                return;
            }
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
            //delete userX;
            elite->setGodPower(this);
            // std::string *paramsRep;
            // paramsRep = allocateForParams(2);
            // paramsRep[0] = userX->getNickForReply();
            // paramsRep[1] = ":A God among the living";
            // sendReply(userX->sendFd, this->serverName, RPL_YOUREOPER, paramsRep);
            // delete [] paramsRep;
            this->sendHermes(this->sendNumericCode(userX, NULL, RPL_YOUREOPER, "GODLY POWERS! HERE WE GO!"), send);
        }
        else{
            // std::string *paramsRep;
            // paramsRep = allocateForParams(2);
            // paramsRep[0] = userX->getNickForReply();
            // paramsRep[1] = ":invalid credentials";
            // sendReply(userX->sendFd, this->serverName, ERR_PASSWDMISMATCH, paramsRep);
            // delete [] paramsRep;
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
    //std::string *params;
    std::vector<User *> send;
    send.push_back(userX);
    if (paramNumber == 1 && param[1].find("#") != std::string::npos){ // MODE in user mode! change the 6 to a 1
       //params = allocateForParams(1);
       // std::cout << "here from mirican" << std::endl;
        //if (param[1].find("#") != std::string::npos){
            //params[0] = getChannelPriv(param[1]);
        this->sendHermes(this->sendNumericCode(userX, NULL, RPL_CHANNELMODEIS, std::string(getChannelPriv(param[1]))), send);
        //}
        // this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODEIS, params);
        // delete [] params;
        return;
    }
    if (paramNumber > 5){
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply();
        // params[1] = ": Not enough Parameters!";
        // this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
        // delete [] params;
        this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "not enough parameters!"), send);
        return;
    }
    int check = checkStringToken(param, paramNumber);
    if (check > 2){
        //Error of +- or too many Channels;
        if (paramNumber == 2 && param[1] == userX->getNickForReply()){
            // params = allocateForParams(1);
            // params[0] = "Invisible mode on!";
            // this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODEIS, params);
            // delete [] params;
            this->sendHermes(this->sendNumericCode(userX, NULL, RPL_UMODEIS, "+i"), send);
            return;
        }
        else{
            // params = allocateForParams(2);
            // params[0] = userX->getNickForReply();
            // params[1] = ": Syntax Error!";
            // this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
            // delete [] params;
            this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "Syntax Error"), send);
            return;
        }
    }
    else{
        switch (check){
            case 1:{
                // Error, no channel given
                // params = allocateForParams(2);
                // params[0] = userX->getNickForReply();
                // params[1] = ":No Channel has been specified!";
                // this->sendReply(userX->sendFd, this->serverName, ERR_USERNOTINCHANNEL, params);
                // delete [] params;
                this->sendHermes(this->sendNumericCode(userX, NULL, ERR_USERNOTINCHANNEL, "Channel not specified"), send);
                return;
            }
            case 2:{
                Channel *currentCh = userX->channelFinderinUser(param[1]);
                if (currentCh){ // check is empty param[1] mohim hiya li fiha l mochkil
                    // treat as current Channel
                    this->handleCmdModeOpt(param, userX, currentCh);
                }
                else{
                    //std::cout << "haaaa fin o haaaaaa fin 3" << std::endl;
                    // treat as external Channel
                    Channel *find = this->channelFinder(param[1]);
                    if (!find){
                        //Error Channel doesnt exist
                        // params = allocateForParams(1);
                        // params[0] = userX->getNickForReply();
                        // params[1] = ": Channel does not exist!";
                        // this->sendReply(userX->sendFd, this->serverName, ERR_USERNOTINCHANNEL, params);
                        // delete [] params;
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
    //std::string *param;
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
                // params = allocateForParams(2);
                // params[0] = userX->getNickForReply() + " " + Ch->channelName;
                // params[1] = ":Not enough parameter for the options specified!";
                // this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
                // delete [] params;
                this->sendHermes(this->sendNumericCode(userX, Ch, ERR_NEEDMOREPARAMS, "Not enough parameters for the option specified"), send);
                return;
            }
            options.insert(std::make_pair(params[2].c_str()[i], params[paramIndex]));
        }
        else if(params[2].c_str()[i] == 't' || params[2].c_str()[i] == 'i')
            options.insert(std::make_pair(params[2].c_str()[i], params[1]));
        else{
            // Error mode option not correct
            // params = allocateForParams(2);
            // params[0] = userX->getNickForReply() + " " + Ch->channelName;
            // param[1] = "The Options specified are not correct! \" itkol \"";
            // this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
            // delete [] param;
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
   // std::string *params;
    std::vector<User*>  send;
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    send.push_back(userX);
    if (!God && this->findUserinChan(userX->sendFd, chan->channelOps) == -1){
        //Error User is not a chanop
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : You are not a Chanop!";
        // this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPRIVSNEEDED, params);
        // delete [] params;
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_CHANOPRIVSNEEDED, "You are not a chanOp!"), send);
        return;
    }
    if (mode == 0 && !chan->inviteMode){
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : The Invite only Mode is already off!";
        // this->sendReply(userX->sendFd, this->serverName, ERR_INVITEONLYCHAN, params);
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_INVITEONLYCHAN, "Invite Only is already Off!"), send);
    }
    else if (mode == 1 && chan->inviteMode){
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : The Invite only Mode is already On!";
        // this->sendReply(userX->sendFd, this->serverName, ERR_INVITEONLYCHAN, params);
    
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_INVITEONLYCHAN, "Invite only is already On!"), send);
    }
    else if (mode == 0 && chan->inviteMode){
         chan->inviteMode = false;
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : The Invite only Mode is now Off!";
        // this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
        // this->sendGenericReply(userX, "MODE", chan, "-i");
        
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The invite only Mode is now off"), send);
        this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "-i"), chan->channelMembers);
    }
    else if (mode == 1 && !chan->inviteMode){
         chan->inviteMode = true;
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : The Invite only Mode is now On!";
        // this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
        // this->sendGenericReply(userX, "MODE", chan, "+i");
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The invite only Mode is now on"), send);
        this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "+i"), chan->channelMembers);

    }
    // delete [] params;
    return ;
}
void    Server::handleCmdModeOptT(User *userX, Channel *chan, int mode){
        //std::string *params;
        ircGod  *God = dynamic_cast<ircGod*>(userX);
        std::vector<User *> send;
    if (this->findUserinChan(userX->sendFd, chan->channelOps) == -1 && !God){
        //Error User is not a chanop
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : You are not a Chanop!";
        // this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPRIVSNEEDED, params);
        // delete [] params;
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_CHANOPRIVSNEEDED, "You are not a chanOp"), send);
        return;
    }
    if (mode == 0 && !chan->topicProtectMode){
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : The Topic Protection Mode is already off!";
        // this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, params);
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, "The topic protection Mode is already off"), send);
    }
    else if (mode == 1 && chan->topicProtectMode){
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : The Topic Protection Mode is already On!";
        // this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, params);
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, "The topic protection Mode is already off"), send);
    }
    else if (mode == 0 && chan->topicProtectMode){
        chan->topicProtectMode = false;
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : The Topic Protection Mode is now Off!";
        // this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
        // this->sendGenericReply(userX, "MODE", chan, "-t");
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The topic protection Mode is now off"), send);
        this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "-t"), chan->channelMembers);
    }
    else if (mode == 1 && !chan->topicProtectMode){
        chan->topicProtectMode = true;
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : The Topic Protection Mode is now On!";
        // this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
        //this->sendGenericReply(userX, "MODE", chan, "+t");       
        this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The topic protection Mode is now on"), send);
        this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "+t"), chan->channelMembers);
    }
   // delete [] params;
    return ;
}
void    Server::handleCmdModeOptO(User *userX, std::string s, Channel *chan, int mode){
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    User    *player = findUserinServ(s);
    std::vector<User*> send;
    send.push_back(userX);
    //std::string *params;
    if(!God){
        // Error you dont have IRCGod powers.
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : You are not a God!";
        // this->sendReply(userX->sendFd, this->serverName, ERR_NOPRIVILEGES, params);
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_NOPRIVILEGES, "you dont have the powers to do that"), send);
    }
    else if (God && (!player || this->findUserinChan(player->sendFd, chan->channelMembers) == -1)){
        // Error User doesnt exist
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : User does not exist in channel: " + chan->channelName;
        // this->sendReply(userX->sendFd, this->serverName, ERR_USERNOTINCHANNEL, params);
       this->sendHermes(this->sendNumericCode(userX, chan, ERR_USERNOTINCHANNEL, "User specified is not in the channel"), send);        
    }
    else{
        if (mode == 1 && this->findUserinChan(player->sendFd, chan->channelOps) == -1){
            chan->channelOps.push_back(player);
            // params = allocateForParams(2);
            // params[0] = userX->getNickForReply() + " " + chan->channelName;
            // params[1] = ":MODE : You are now a Chanop of " + chan->channelName; 
            // this->sendReply(God->sendFd, this->serverName, RPL_ENDOFBANLIST, params);
            this->sendHermes(this->sendNumericCode(userX, chan, RPL_ENDOFBANLIST, "you are now a chanOp"), send);
        }
        else if (mode == 0 && this->findUserinChan(player->sendFd, chan->channelOps) != -1){
           // params = allocateForParams(2);
            // params[0] = userX->getNickForReply() + " " + chan->channelName;
            // params[1] = ":MODE : You are no longer a Chanop of " + chan->channelName;
            // this->sendReply(God->sendFd, this->serverName, RPL_ENDOFBANLIST, params);
           this->sendHermes(this->sendNumericCode(userX, chan, RPL_ENDOFBANLIST, "you are no longer a chanOp"), send);
        }
        else if (mode == 1 && this->findUserinChan(player->sendFd, chan->channelOps) != 1){
            //params = allocateForParams(2);
            // params[0] = userX->getNickForReply() + " " + chan->channelName;
            // params[1] = ":MODE : " + player->getNickForReply() + " is already a ChanOp!";
            // this->sendReply(player->sendFd, this->serverName, RPL_ENDOFBANLIST, params); // User is already a channel Operator}
           this->sendHermes(this->sendNumericCode(userX, chan, RPL_ENDOFBANLIST, "you are already a chanOp"), send);
        
        }
        else if (mode == 0 && this->findUserinChan(player->sendFd, chan->channelOps) == -1){
            // params = allocateForParams(2);
            // params[0] = userX->getNickForReply() + " " + chan->channelName;
            // params[1] = ":MODE : " + player->getNickForReply() + " is already not a ChanOp!";
            // this->sendReply(player->sendFd, this->serverName, RPL_ENDOFBANLIST, params); // User is not a channel Operator        
           this->sendHermes(this->sendNumericCode(userX, chan, RPL_ENDOFBANLIST, "you are already a chanOp"), send);
        
        }
    }
   // delete [] params;
    return;
}
void    Server::handleCmdModeOptK(User *userX, std::string s, Channel *chan, int mode){
    ircGod  *God = dynamic_cast<ircGod*>(userX);
    std::vector<User*> send;
   // std::string *params;
   send.push_back(userX);
    if (!God && this->findUserinChan(userX->sendFd, chan->channelOps) == -1){
        //Error User is not a chanop
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : You are not a Chanop!";
        // this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPRIVSNEEDED, params);
        // delete [] params;
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_CHANOPRIVSNEEDED, "you are mot a chanOp"), send);
        return;
    }
    if (chan->keyMode){
        switch (mode){
            case 0 :{
                chan->keyMode = false;
                // params = allocateForParams(2);
                // params[0] = userX->getNickForReply() + " " + chan->channelName;
                // params[1] = ":MODE :" + chan->channelName + " no longer has a password!";
                // this->sendReply(userX->sendFd, this->serverName, RPL_INVITING, params);
                // this->sendGenericReply(userX, "MODE", chan, "-k");
                // delete [] params;
                this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODEIS, "Channel no longer has a key"), chan->channelMembers);
                this->sendHermes(this->sendGenericCode(userX, chan, "MODE", "-k"), send);
                break;
            }
            case 1 :{
                chan->keyMode = true;
                chan->setTheKey(s);
                // params = allocateForParams(2);
                // params[0] = userX->getNickForReply() + " " + chan->channelName;
                // params[1] = ":MODE :" + chan->channelName + "has a new password!";
                // this->sendReply(userX->sendFd, this->serverName, RPL_INVITING, params);
                // this->sendGenericReply(userX, "MODE", chan, "+k");
                // delete [] params;
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
                // params = allocateForParams(2);
                // params[0] = userX->getNickForReply() + " " + chan->channelName;
                // params[1] = ":MODE : " + chan->channelName + " already has no password";
                // this->sendReply(userX->sendFd, this->serverName, RPL_NOTOPIC, params);
                // delete [] params;
                this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "The channel already is open!"), chan->channelMembers);
            
                break;
            }
            case 1 :{
                chan->keyMode = true;
                chan->setTheKey(s);
                // params = allocateForParams(2);
                // params[0] = userX->getNickForReply() + " " + chan->channelName;
                // params[1] = "MODE : " +chan->channelName + "has a new password!";
                // this->sendReply(userX->sendFd, this->serverName, RPL_INVITING, params);
                // this->sendGenericReply(userX, "MODE", chan, "+k");
                // delete [] params;
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
    //std::string *params;
    if (!God && this->findUserinChan(userX->sendFd, chan->channelOps) == -1){
        //Error User is not a chanop
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = ":MODE : You are not a Chanop!";
        // this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPRIVSNEEDED, params);
        // delete [] params;
       this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "you are not a chanOp"), send);
        return;
    }
    bool    check = checkNums(s);
    if (check){
        size_t limit = stoi(s);
        if (chan->limitMode){
            switch (mode){
                case 0 :{
                    chan->limitMode = false; // error there s no limit 
                    // params = allocateForParams(2);
                    // params[0] = userX->getNickForReply() + " " + chan->channelName;
                    // params[1] = "MODE :" + chan->channelName + " is no longer limited!";
                    // this->sendReply(userX->sendFd, this->serverName, RPL_NOTOPIC, params);
                    // this->sendGenericReply(userX, "MODE", chan, "-l");
                    // delete [] params;
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
                    // params = allocateForParams(2);
                    // params[0] = userX->getNickForReply() + " " + chan->channelName;
                    // params[1] = ":MODE :" + chan->channelName + "has a new limit!"; 
                    // this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
                    // this->sendGenericReply(userX, "MODE", chan, "+");
                    // delete [] params;
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
                    // params = allocateForParams(2);
                    // params[0] = userX->getNickForReply() + " " + chan->channelName;
                    // params[1] = ":MODE :" + chan->channelName + " already has no limit";
                    // this->sendReply(userX->sendFd, this->serverName, RPL_NOTOPIC, params);
                    // delete [] params;
                    this->sendHermes(this->sendNumericCode(userX, chan, RPL_CHANNELMODES, "channel already has no limit"), send);
                    break;
                }
                case 1 :{
                    if (limit == 0)
                        chan->limitMode = false;
                    else
                        chan->limitMode = true;
                    chan->setTheLimit(limit);
                    // params = allocateForParams(2);
                    // params[0] = userX->getNickForReply() + " " + chan->channelName;
                    // params[1] = "MODE" + chan->channelName + "has a new limit!";
                    // this->sendReply(userX->sendFd, this->serverName, RPL_CHANNELMODES, params);
                    // this->sendGenericReply(userX, "MODE", chan, "+l");
                    // delete [] params;
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
        //Error here for non numeric argument
        // params = allocateForParams(2);
        // params[0] = userX->getNickForReply() + " " + chan->channelName;
        // params[1] = " MODE : Your argument is non numeric!"; 
        // this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, params);
        // delete [] params;
        this->sendHermes(this->sendNumericCode(userX, chan, ERR_NEEDMOREPARAMS, "Your parameter is non numeric"), send);
    }
    return;
}

