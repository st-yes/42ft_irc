# include "../includes/Ft_irc.hpp"

void Server::handleCmdQuit(std::string	*params, User *userX ,int paramNumber)
{   
    Channel *chan;
    int index = 0;
    int indexchanOp = 0;
    std::string reply = "";
    if (!params[1].empty())
        reply = params[1];
	for (int i = 0; i != userX->joinedChannels.size(); i++){
        chan = userX->joinedChannels[i];
        indexchanOp = this->findUserinChan(userX->sendFd, chan->channelOps);
        index = this->findUserinChan(userX->sendFd, chan->channelMembers);
            if (index == -1)
                continue;
        chan->channelMembers.erase(chan->channelMembers.begin() + index);
        if (indexchanOp != -1)
            chan->channelOps.erase(chan->channelOps.begin() + indexchanOp);
        this->sendHermes(this->sendGenericCode(userX, chan, "PART", reply), chan->channelMembers);
        this->sendHermes(this->sendGenericCode(userX, chan, "QUIT", reply), chan->channelMembers);
    }
    this->lostConnection(userX);
}