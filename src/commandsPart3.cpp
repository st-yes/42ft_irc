#include "../includes/Ft_irc.hpp"

void    Server::handleCmdTopic(std::string *params, User *userX, int paramNumber){
    std::vector<User*> send;
    send.push_back(userX);
    Channel *chan = this->channelFinder(params[1]);
    if (paramNumber > 3){
        this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NEEDMOREPARAMS, "too many parameters!"), send);
    }
    else{
        if (!chan){
            this->sendHermes(this->sendNumericCode(userX, NULL, ERR_NOSUCHCHANNEL, "there s not such channel"), send);
            return ;
        }
        else{
            int k = findUserinChan(userX->sendFd, chan->channelMembers);
            if (k != -1){
                if (!params[2].empty()){
                    if (chan->topicProtectMode){
                        if (findUserinChan(userX->sendFd, chan->channelOps) != -1){
                            chan->channelTopic = params[2];
                            int i = 3;
                            while (params[i] != ""){
                                chan->channelTopic += params[i++];
                                chan->channelTopic += " ";
                            }
                            this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, chan->channelTopic), send);
                            this->sendHermes(this->sendGenericCode(userX, chan, "TOPIC", ""), chan->channelMembers);
                            return ;
                        }
                        else {
                            this->sendHermes(this->sendNumericCode(userX, chan, ERR_CHANOPRIVSNEEDED, "you are not a chanOp!"), send);
                            return ;
                        }
                    }
                    else{
                        chan->channelTopic = params[2];
                        int i = 3;
                        while (params[i] != ""){
                            chan->channelTopic += params[i++];
                            chan->channelTopic += " ";
                        }
                        this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, chan->channelTopic), send);
                        this->sendHermes(this->sendGenericCode(userX, chan, "TOPIC", ""), chan->channelMembers);
                        return ;
                    }
                }
                else{
                    this->sendHermes(this->sendNumericCode(userX, chan, RPL_TOPIC, chan->channelTopic), send);
                    this->sendHermes(this->sendGenericCode(userX, chan, "TOPIC", ""), chan->channelMembers);
                    return ;
                }
            }
            else{
                    this->sendHermes(this->sendNumericCode(userX, chan, RPL_NOTOPIC, ""), send);
                    return ;
            }
        }
    }
    return ;
}

