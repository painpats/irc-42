#include "Commands.hpp"

int    checkChannelMode(User &user, Channel &channel, std::vector<std::string> password, size_t i)
{
    if (channel.getModeInvite() == true)
    {
        std::map<int, User>::iterator itUser = channel.getInvitations().find(user.getSocket());
        if (itUser == channel.getInvitations().end())
        {
            ERR_INVITEONLYCHAN(user.getSocket(), user.getNickname(), channel.getName());
            return (1);
        }
    }
    if (channel.getModeLimit() == true && channel.getUserLimit() == channel.getNbUser())
    {
        ERR_CHANNELISFULL(user.getSocket(), user.getNickname(), channel.getName());
            return (1);
    }
    if (channel.getModeKey() == true)
    {
        if (password.size() > i)
        {
            if (channel.getPassword() != password[i])
            {
                ERR_BADCHANNELKEY(user.getSocket(), user.getNickname(), channel.getName());
                return (1);
            }
        }
        else
        {
            ERR_BADCHANNELKEY(user.getSocket(), user.getNickname(), channel.getName());
            return (1);
        }
    }
    return (0);
}

void    joinChannel(User &user, Channel &channel, std::vector<std::string> password, int i)
{
    if (channel.getUsers().find(user.getSocket()) == channel.getUsers().end())
    {
        if (checkChannelMode(user, channel, password, i))
            return ;
        channel.addUser(user.getSocket(), user);
        std::string send_msg = ":" + user.getNickname() + "!" + user.getUsername() + " JOIN " + channel.getName();
        sendMsgAll(channel.getUsers(), send_msg, "");
        sendMsgAll(channel.getUsers(), nameReply(channel, user), "");
        if (channel.getTopic() != "")
            RPL_TOPIC(user.getSocket(), user.getNickname(), channel.getName(), channel.getTopic());
    }
}

void    createChannel(User &user, Channel &channel, std::string channelName)
{
    channel.setName(channelName);
    channel.addUser(user.getSocket(), user);
    channel.addOperator(user.getSocket(), user);
    channel.setUserLimit(0);
    std::string send_msg = ":" + user.getNickname() + "!" + user.getUsername() + " JOIN " + channel.getName();
    sendMsg(send_msg, user.getSocket());
    sendMsgAll(channel.getUsers(), nameReply(channel, user), "");
}

void leaveAllChannels(User &user, std::map<std::string, Channel>&channelList)
{
    std::map<std::string, Channel>::iterator it = channelList.begin();
    while (it != channelList.end())
    {
        std::map<int, User>::iterator itUser = it->second.getUsers().find(user.getSocket());
        if (itUser != it->second.getUsers().end())
        {
            it->second.getUsers().erase(itUser);
            it->second.getUserOperators().erase(user.getSocket());
            std::string partMsg = ":" + user.getNickname() + " PART " + it->second.getName();
            sendMsgAll(it->second.getUsers(), partMsg, "");
            sendMsgAll(it->second.getUsers(), nameReply(it->second, user), "");
        }
        if (it->second.getUsers().empty())
        {
            channelList.erase(it);
            it = channelList.begin();
        }
        else
            it++;
    }
}

void    join(std::string cmd, User &user, std::map<std::string, Channel>&channelList)
{
    Utils utils;
    std::vector<std::string> args = utils.split(cmd, ' ');
    if (args.size() == 1)
    {
        ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), cmd);
        return;
    }
    std::vector<std::string> channelName = utils.split(args[1], ',');
    std::vector<std::string> password;
    if (args.size() > 2)
        password = utils.split(args[2], ',');
    if (channelName.size() == 1 && channelName[0] == "0")
    {
        leaveAllChannels(user, channelList);
        return;
    }
    int index = 0;
    for (size_t i = 0; i < channelName.size(); i++)
    {
        if (channelName[i][0] != '#')
        {
            ERR_NOSUCHCHANNEL(user.getSocket(), user.getNickname(), channelName[i]);
            return;
        }
        if (utils.IsValidAlphaNum(channelName[i].substr(1)) == false)
        {
            ERR_BADCHANNELNAME(user.getSocket(), user.getNickname(), channelName[i]);
            return;
        }
        std::map<std::string, Channel>::iterator itChannel = channelList.find(channelName[i]);
        if (itChannel != channelList.end())
        {
            joinChannel(user, channelList[channelName[i]], password, index);
            index++;
        }
        else
        {
            channelList[channelName[i]] = Channel();
            createChannel(user, channelList[channelName[i]], channelName[i]);
        }
    }
}