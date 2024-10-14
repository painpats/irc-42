#include "Commands.hpp"

template <typename T>
std::string NumberToString(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

std::string    getModes(Channel &channel)
{
    std::string modes = "+";
    if (channel.getModeKey())
        modes += "k";
    if (channel.getModeTopic())
        modes += "t";
    if (channel.getModeLimit())
        modes += "l";
    if (channel.getModeInvite())
        modes += "i";
    if (modes == "+")
        return ("+");
    if (channel.getModeKey())
        modes += " " + channel.getPassword();
    if (channel.getModeLimit())
        modes += " " + NumberToString(channel.getUserLimit());
    return (modes);
}

void    printMode(std::vector<std::string>cmd, User &user, std::map<std::string, Channel>&channelList)
{
    std::string channelName = cmd[1];
    std::map<std::string, Channel>::iterator itChan = channelList.find(channelName);
    if (itChan != channelList.end())
    {
        if (itChan->second.getUsers().find(user.getSocket()) == itChan->second.getUsers().end())
            return (ERR_NOTONCHANNEL(user.getSocket(), user.getNickname(), itChan->second.getName()));
        std::string modes = getModes(itChan->second);
        if (modes != "+")
            RPL_CHANNELMODEIS(user.getSocket(), user.getNickname(), itChan->second.getName(), modes);
        else
            ERR_NOCHANMODES(user.getSocket(), user.getNickname(), itChan->second.getName());
    }
    else
        ERR_NOSUCHCHANNEL(user.getSocket(), user.getNickname(), channelName);
}

void    mode(std::string cmd, User &user, std::map<std::string, Channel>&channelList)
{
    Utils utils;
    std::vector<std::string> args = utils.split(cmd, ' ');
    if (args.size() < 2)
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), cmd));
    if (args.size() == 2)
        return (printMode(args, user, channelList));
    else if (args.size() < 3)
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), cmd));
    std::map<std::string, Channel>::iterator itChan = channelList.find(args[1]);
    if (itChan == channelList.end())
        return (ERR_NOSUCHCHANNEL(user.getSocket(), user.getNickname(), args[1]));
    else
    {
        if (itChan->second.getUsers().find(user.getSocket()) == itChan->second.getUsers().end())
            return (ERR_NOTONCHANNEL(user.getSocket(), user.getNickname(), itChan->second.getName()));
        if (itChan->second.getUserOperators().find(user.getSocket()) == itChan->second.getUserOperators().end())
            return (ERR_CHANOPRIVSNEEDED(user.getSocket(), user.getNickname(), itChan->second.getName()));
    }
    if (unknownFlags(args[2]) == false)
        return (ERR_UNKNOWNMODE(user.getSocket(), user.getNickname(), args[2], itChan->second.getName()));
    std::multimap<char, char> allFlags = fillFlags(args[2]);
    std::string Password = "";
    std::list<std::string> Nickname;
    int limitUsers = 0;
    sortFlags(&Password, &Nickname, &limitUsers, args, itChan->second.getUsers());
    changeModes(allFlags, Password, Nickname, limitUsers, itChan->second, user);
    if (getModes(itChan->second) != "+")
        RPL_CHANNELMODEIS(user.getSocket(), user.getNickname(), itChan->second.getName(), getModes(itChan->second));
    else
        ERR_NOCHANMODES(user.getSocket(), user.getNickname(), itChan->second.getName());
}