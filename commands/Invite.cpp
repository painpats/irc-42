#include "Commands.hpp"

void    invite(std::string cmd, User &user, std::map<int, User>&userList, std::map<std::string, Channel>&channelList)
{
    Utils utils;
    std::vector<std::string>args = utils.split(cmd, 32);
    if (args.size() < 3)
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), cmd));
    std::map<int, User>::iterator itUser = userList.begin();
    std::map<std::string, Channel>::iterator itChan = channelList.find(args[2]);
    while (itUser != userList.end())
    {
        if (itUser->second.getNickname() == args[1])
            break ;
        itUser++;
    }
    if (itUser == userList.end())
        return(ERR_NOSUCHNICK(user.getSocket(), args[1]));
    if (itChan != channelList.end())
    {
        if (itChan->second.getUsers().find(user.getSocket()) == itChan->second.getUsers().end())
            return (ERR_NOTONCHANNEL(user.getSocket(), user.getNickname(), itChan->second.getName()));
        if (itChan->second.getModeInvite() == true)
        {
            if (itChan->second.getUserOperators().find(user.getSocket()) == itChan->second.getUserOperators().end())
                return (ERR_CHANOPRIVSNEEDED(user.getSocket(), user.getNickname(), itChan->second.getName()));
            if (itChan->second.getUsers().find(itUser->second.getSocket()) == itChan->second.getUsers().end())
                itChan->second.addInvitation(itUser->second.getSocket(), itUser->second);
            else
                return (ERR_USERONCHANNEL(user.getSocket(), user.getNickname(), itUser->second.getNickname(), itChan->second.getName()));
        }
        else
        {
            if (itChan->second.getUsers().find(itUser->second.getSocket()) != itChan->second.getUsers().end())
                return (ERR_USERONCHANNEL(user.getSocket(), user.getNickname(), itUser->second.getNickname(), itChan->second.getName()));
        }
        if (itUser->second.getRegister() == false)
            return ERR_NOSUCHNICK(user.getSocket(), user.getNickname());
        std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + " INVITE " + itUser->second.getNickname() + " " + args[2];
        sendMsg(msg.c_str(), itUser->second.getSocket());
        RPL_INVITING(user.getSocket(), user.getNickname(), args[2], itUser->second.getNickname());
    }
    else
        ERR_NOSUCHCHANNEL(user.getSocket(), user.getNickname(), args[2]);
}
