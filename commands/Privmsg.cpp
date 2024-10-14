#include "Commands.hpp"

void    privNick(User &user, std::map<int, User>&userList, std::string nick, std::string txt)
{
    std::map<int, User>::iterator it = userList.begin();
    while (it != userList.end())
    {
        if (it->second.getNickname() == nick)
            break ;
        it++;
    }
    if (it == userList.end())
        return (ERR_NOSUCHNICK(user.getSocket(), nick));
    if (it->second.getRegister() == false)
        return ERR_NOSUCHNICK(user.getSocket(), user.getNickname());
    std::string send_msg = ":" + user.getNickname() + "!" + user.getUsername() + " PRIVMSG " + it->second.getNickname() + " :" + txt;
    sendMsg(send_msg, it->second.getSocket());
}

void    privChan(User &user, std::map<std::string, Channel>&channelList, std::string channelName, std::string txt)
{
    if (channelList.find(channelName) == channelList.end())
        return (ERR_NOSUCHCHANNEL(user.getSocket(), user.getNickname(), channelName));
    else if (channelList.find(channelName) != channelList.end())
    {
        if (channelList[channelName].getUsers().find(user.getSocket()) == channelList[channelName].getUsers().end())
            return (ERR_CANNOTSENDTOCHAN(user.getSocket(), user.getNickname(), channelName));
    }
    std::string send_msg = ":" + user.getNickname() + "!" + user.getUsername() + " PRIVMSG " + channelName + " :" + txt;
    sendMsgAll(channelList[channelName].getUsers(), send_msg, user.getNickname());
}

std::string    privText(std::string cmd)
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

void    privmsg(std::string cmd, User &user, std::map<int, User>&userlist, std::map<std::string, Channel>&channelList)
{
    Utils utils;
    std::vector<std::string> args = utils.split(cmd, 32);
    if (args.size() < 3)
        return (ERR_NOTEXTTOSEND(user.getSocket(), user.getNickname()));
    std::string txt = privText(cmd);
    if (txt == "")
        return (ERR_NOTEXTTOSEND(user.getSocket(), user.getNickname()));
    if (args[1][0] == '#')
        privChan(user, channelList, args[1], txt);
    else
        privNick(user, userlist, args[1], txt);
}
