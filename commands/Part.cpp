#include "Commands.hpp"

std::string    partText(std::string cmd)
{
    int space = 0;
    int i = 0;
    std::string text;
    while (cmd[i])
    {
        if (cmd[i] == 32 && space < 2)
        {
            space += 1;
            while (cmd[i] && cmd[i] == 32)
                i++;
        }
        else if (space == 2)
        {
            text += cmd[i];
            i++;
        }
        else
            i++;
    }
    if (text[0] == ':')
        text.erase(0, 1);
    return (text);
}

void    part(std::string cmd, User &user, std::map<std::string, Channel>&channelList)
{
    Utils utils;
    std::vector<std::string>args = utils.split(cmd, 32);
    if (args.size() < 2)
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), cmd));
    std::vector<std::string>channelsCmd = utils.split(args[1], ',');
    std::vector<std::string>channels;
    for (size_t i = 0; i < channelsCmd.size(); i++)
    {
        if (channelList.find(channelsCmd[i]) != channelList.end())
            channels.push_back(channelsCmd[i]);
        else
            ERR_NOSUCHCHANNEL(user.getSocket(), user.getNickname(), channelsCmd[i]);
    }
    if (channels.empty())
        return ;
    std::string txt = partText(cmd);
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::map<std::string, Channel>::iterator itChan = channelList.find(channels[i]);
        if (itChan->second.getUsers().find(user.getSocket()) != itChan->second.getUsers().end())
        {
            if (txt == "")
            {
                if (txt[0] == ':')
                    txt.erase(0, 1);
                std::string partMsg = ":" + user.getNickname() + " PART " + itChan->second.getName();
                sendMsgAll(itChan->second.getUsers(), partMsg, "");
            }
            else
            {
                std::string partMsg = ":" + user.getNickname() + " PART " + itChan->second.getName() + " " + txt;
                sendMsgAll(itChan->second.getUsers(), partMsg, "");
            }
            itChan->second.removeUser(user.getSocket());
            if (itChan->second.getUsers().empty())
                channelList.erase(itChan);
            else
                sendMsgAll(itChan->second.getUsers(), nameReply(itChan->second, user), user.getNickname());
        }
        else
            ERR_NOTONCHANNEL(user.getSocket(), user.getNickname(), itChan->second.getName());
    }
}
