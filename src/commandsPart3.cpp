#include "../includes/Ft_irc.hpp"

void    Server::handleCmdTopic(std::string *params, User *userX, int paramNumber){
    std::string *param;
    Channel *chan = this->channelFinder(params[1]);
    if (paramNumber > 3){
        // Err too many Params
        param = allocateForParams(1);
        param[0] = userX->getNickForReply() + " Too many Params";
        this->sendReply(userX->sendFd, this->serverName, ERR_NEEDMOREPARAMS, param);
    }
    else{
        if (!chan){
        // Err Channel doesnt exist
        param = allocateForParams(1);
        param[0] = userX->getNickForReply() + " No such channel";
        this->sendReply(userX->sendFd, this->serverName, ERR_NOSUCHCHANNEL, param);
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
                            param = allocateForParams(1);
                            param[0] = userX->getNickForReply() + " " + chan->channelName + " " + chan->channelTopic;
                            this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, param);
                            this->sendGenericReply(userX, "TOPIC", chan, "");
                        }
                        else {
                            // Err is not chanop
                            param = allocateForParams(1);
                            param[0] = userX->getNickForReply() + " " + chan->channelName + " :You are not Chanop!";
                            this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPRIVSNEEDED, param);
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
                        param = allocateForParams(1);
                        param[0] = userX->getNickForReply() + " " + chan->channelName + " " + chan->channelTopic;
                        this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, param);
                        this->sendGenericReply(userX, "TOPIC", chan, "");
                    }
                }
                else{
                    // Case of printing topic
                    param = allocateForParams(1);
                    param[0] = userX->getNickForReply() + " " + chan->channelName + " " + chan->channelTopic;
                    this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, param);
                    this->sendGenericReply(userX, "TOPIC", chan, "");
                }
            }
            else{
                    // Err User doesnt exist
                    param = allocateForParams(1);
                    param[0] = userX->getNickForReply() + " " + chan->channelName + " :NO TOPIC IS SET";
                    this->sendReply(userX->sendFd, this->serverName, RPL_NOTOPIC, param);
            }
        }
    }
    delete [] param;
    return ;
}

