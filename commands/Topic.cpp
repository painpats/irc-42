#include "Commands.hpp"

std::string    topicText(std::string cmd)
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
    return (text);
}

void    changeTopic(User &user, Channel &channel, std::string topic)
{
    if (topic[0] == ':')
        topic.erase(0, 1);
    if (channel.getModeTopic() == true)
    {
        if (channel.getUsers().find(user.getSocket()) == channel.getUsers().end())
            return (ERR_NOTONCHANNEL(user.getSocket(), user.getNickname(), channel.getName()));
        if (channel.getUserOperators().find(user.getSocket()) == channel.getUserOperators().end())
            return (ERR_CHANOPRIVSNEEDED(user.getSocket(), user.getNickname(), channel.getName()));
    }
    else if (channel.getUsers().find(user.getSocket()) == channel.getUsers().end())
            return (ERR_NOTONCHANNEL(user.getSocket(), user.getNickname(), channel.getName()));
    if (topic[0] == ':')
        channel.setTopic(topic.substr(1));
    else
        channel.setTopic(topic);
    std::string msg = ":" + user.getNickname() + " TOPIC " + channel.getName() + " " + topic;
    sendMsgAll(channel.getUsers(), msg, "");
}

void    removeTopic(User &user, Channel &channel)
{
    if (channel.getModeTopic() == true)
    {
        if (channel.getUsers().find(user.getSocket()) == channel.getUsers().end())
            return (ERR_NOTONCHANNEL(user.getSocket(), user.getNickname(), channel.getName()));
        if (channel.getUserOperators().find(user.getSocket()) == channel.getUserOperators().end())
            return (ERR_CHANOPRIVSNEEDED(user.getSocket(), user.getNickname(), channel.getName()));
    }
    else if (channel.getUsers().find(user.getSocket()) == channel.getUsers().end())
            return (ERR_NOTONCHANNEL(user.getSocket(), user.getNickname(), channel.getName()));
    channel.setTopic("");
    std::string msg = ":" + user.getNickname() + " TOPIC " + channel.getName() + " " + "";
    sendMsgAll(channel.getUsers(), msg, "");
}

void    printTopic(User &user, Channel &channel)
{
    if (channel.getTopic() != "")
        RPL_TOPIC(user.getSocket(), user.getNickname(), channel.getName(), channel.getTopic());
    else
        RPL_NOTOPIC(user.getSocket(), user.getNickname(), channel.getName());
}

void        topic(std::string cmd, User &user, std::map<std::string, Channel>&channelList)
{
    Utils utils;
    
    std::vector<std::string> args = utils.split(cmd, ' ');
    if (args.size() < 2)
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), cmd));
    std::map<std::string, Channel>::iterator itChan = channelList.find(args[1]);
    if (itChan == channelList.end())
        return (ERR_NOSUCHCHANNEL(user.getSocket(), user.getNickname(), args[1]));
    std::string topic = topicText(cmd);
    if (args.size() == 2)
        printTopic(user, itChan->second);
    else if (args.size() == 3 && topic == ":")
        removeTopic(user, itChan->second);
    else
        changeTopic(user, itChan->second, topic);
}