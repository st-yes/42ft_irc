#include "../includes/Ft_irc.hpp"

void    Server::handleCmdTopic(std::string *params, User *userX, int paramNumber){
    //std::string        *param;
    std::vector<User*> send;
    send.push_back(userX);
    Channel *chan = this->channelFinder(params[1]);
    if (paramNumber > 3){
        // Err too many Params
        // param = allocateForParams(1);
        // param[0] = userX->getNickForReply() + " Too many Params";
        // this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, param);
        this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "too many parameters!"), send);
    }
    else{
        if (!chan){
        // Err Channel doesnt exist
        // param = allocateForParams(1);
        // param[0] = userX->getNickForReply() + " No such channel";
        // this->sendReply(userX->sendFd, this->serverName, ERR_NOSUCHCHANNEL, param);
            this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHCHANNEL, "there s not such channel"), send);
            return ;
        }
        else{
            int k = findUserinChan(userX->sendFd, chan->channelMembers);
            if (k != -1){
                if (params[2] != ""){
                    if (chan->topicProtectMode){
                        if (findUserinChan(userX->sendFd, chan->channelOps) != -1){
                            // send correct answer change topic
                            chan->channelTopic = params[2];
                            int i = 3;
                            while (params[i] != ""){
                                chan->channelTopic += params[i++];
                                chan->channelTopic += " ";
                            }
                            // param = allocateForParams(1);
                            // param[0] = userX->getNickForReply() + " " + chan->channelName + " " + chan->channelTopic;
                            //this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, param);
                            //this->sendGenericReply(userX, "TOPIC", chan, "");
                            this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, chan->channelTopic), send);
                            this->sendHermes(this->sendGenericCode(userX, chan, "TOPIC", ""), chan->channelMembers);
                            return ;
                        }
                        else {
                            // Err is not chanop
                            // param = allocateForParams(1);
                            // param[0] = userX->getNickForReply() + " " + chan->channelName + " :You are not Chanop!";
                            // this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPRIVSNEEDED, param);
                            this->sendHermes(this->sendNumericCode(userX, chan, ERR_CHANOPRIVSNEEDED, "you are not a chanOp!"), send);
                            return ;
                        }
                    }
                    else{
                        // Case change Topic
                        chan->channelTopic = params[2];
                        int i = 3;
                        while (params[i] != ""){
                            chan->channelTopic += params[i++];
                            chan->channelTopic += " ";
                        }
                        //param = allocateForParams(1);
                        // param[0] = userX->getNickForReply() + " " + chan->channelName + " " + chan->channelTopic;
                        // this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, param);
                        // this->sendGenericReply(userX, "TOPIC", chan, "");
                        this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, chan->channelTopic), send);
                        this->sendHermes(this->sendGenericCode(userX, chan, "TOPIC", ""), chan->channelMembers);
                        return ;
                    }
                }
                else{
                    // Case of printing topic
                    // param = allocateForParams(1);
                    // param[0] = userX->getNickForReply() + " " + chan->channelName + " " + chan->channelTopic;
                    // this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, param);
                    // this->sendGenericReply(userX, "TOPIC", chan, "");
                    this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, chan->channelTopic), send);
                    this->sendHermes(this->sendGenericCode(userX, chan, "TOPIC", ""), chan->channelMembers);
                    return ;
                }
            }
            else{
                    // Err User doesnt exist
                    // param = allocateForParams(1);
                    // param[0] = userX->getNickForReply() + " " + chan->channelName + " :NO TOPIC IS SET";
                    // this->sendReply(userX->sendFd, this->serverName, RPL_NOTOPIC, param);
                    this->sendHermes(this->sendNumericCode(userX, chan, RPL_NOTOPIC, ""), send);
                    return ;
            }
        }
    }
    //delete [] param;
    return ;
}

