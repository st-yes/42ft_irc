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
        std::cout << "hna2" << std::endl;
        param[0] = userX->getNickForReply() + " No such channel";
        this->sendReply(userX->sendFd, this->serverName, ERR_NOSUCHCHANNEL, param);
        }
    else{
            std::cout << "hna3" << std::endl;
            int k = findUserinChan(userX->sendFd, chan->channelMembers);
            if (k != -1){
                std::cout << "hna4" << std::endl;
                if (params[2] != ""){
                    std::cout << "hna5" << std::endl;
                    if (chan->topicProtectMode){
                        std::cout << "hna6" << std::endl;
                        if (findUserinChan(userX->sendFd, chan->channelOps) != -1){
                            std::cout << "hna7" << std::endl;
                            // send correct answer change topic
                            std::string topic = params[2].substr(1, params[2].length());
                            chan->channelTopic = topic;
                            param = allocateForParams(1);
                            param[0] = userX->getNickForReply() + " " + chan->channelName + " " + chan->channelTopic;
                            this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, param);
                        }
                        else {
                            std::cout << "hna8" << std::endl;
                            // Err is not chanop
                            param = allocateForParams(1);
                            param[0] = userX->getNickForReply() + " " + chan->channelName + " :You are not Chanop!";
                            this->sendReply(userX->sendFd, this->serverName, ERR_CHANOPPRIVNEEDED, param);
                        }
                    }
                    else{
                        std::cout << "hna9" << std::endl;
                        // Case change Topic
                        std::string topic = params[2].substr(1, params[2].length());
                        chan->channelTopic = topic;
                        param = allocateForParams(1);
                        param[0] = userX->getNickForReply() + " " + chan->channelName + " " + chan->channelTopic;
                        this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, param);
                    }
                }
                else{
                    std::cout << "hna10" << std::endl;
                    // Case of printing topic
                    std::cout << "hopla :" + chan->channelTopic << std::endl;
                    param = allocateForParams(1);
                    param[0] = userX->getNickForReply() + " " + chan->channelName + " " + chan->channelTopic;
                    this->sendReply(userX->sendFd, this->serverName, RPL_TOPIC, param);
                }
            }
            else{
                std::cout << "hna11" << std::endl;
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

