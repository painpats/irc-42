#include "Commands.hpp"
#include "../utils/Utils.hpp"

void usageCommand(std::string cmd, User &user)
{
    Utils utils;
    if (!strncmp(utils.toLowerCase(cmd).c_str(), "pass ", 5) || utils.toLowerCase(cmd) == "pass")
        ERR_ALREADYREGISTRED(user.getSocket(), user.getNickname());
    else if (!strncmp(utils.toLowerCase(cmd).c_str(), "nick ", 5) || utils.toLowerCase(cmd) == "nick")
        ERR_ALREADYREGISTRED(user.getSocket(), user.getNickname());
    else if (!strncmp(utils.toLowerCase(cmd).c_str(), "user ", 5) || utils.toLowerCase(cmd) == "user")
        ERR_ALREADYREGISTRED(user.getSocket(), user.getNickname());
    else if (utils.toLowerCase(cmd) == "join")
        ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("JOIN"));
    else if (utils.toLowerCase(cmd) == "kick")
        ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("KICK"));
    else if (utils.toLowerCase(cmd) == "part")
        ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("PART"));
    else if (utils.toLowerCase(cmd) == "privmsg")
        ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("PRIVMSG"));
    else if (utils.toLowerCase(cmd) == "mode")
        ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("MODE"));
    else if (utils.toLowerCase(cmd) == "topic")
        ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("TOPIC"));
    else if (utils.toLowerCase(cmd) == "invite")
        ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("INVITE"));
    else if (utils.toLowerCase(cmd) == "quit")
        ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("QUIT"));
    else
        ERR_UNKNOWNCOMMAND(user.getSocket(), user.getNickname(), cmd);
}

int handlerCommand(std::string buffer, int socket, Server &server)
{
    Utils utils;

    std::vector<std::string> sstr = utils.split(buffer, '\n');
    if (server.getUserList()[socket].getRegister() == false)
    {
        checkRegister(server, socket, sstr);
        return (0);
    }
    std::string cmd = utils.removeCRLF(buffer);
    if (!cmd.empty())
    {
        if (!strncmp(utils.toLowerCase(cmd).c_str(), "join ", 5))
            join(cmd, server.getUserList()[socket], server.getChannelList());
        else if (!strncmp(utils.toLowerCase(cmd).c_str(), "kick ", 5))
            kick(cmd, server.getUserList()[socket], server.getChannelList(), server);
        else if (!strncmp(utils.toLowerCase(cmd).c_str(), "part ", 5))
            part(cmd, server.getUserList()[socket], server.getChannelList());
        else if (!strncmp(utils.toLowerCase(cmd).c_str(), "privmsg ", 8))
            privmsg(cmd, server.getUserList()[socket], server.getUserList(), server.getChannelList());
        else if (!strncmp(utils.toLowerCase(cmd).c_str(), "mode ", 5))
            mode(cmd, server.getUserList()[socket], server.getChannelList());
        else if (!strncmp(utils.toLowerCase(cmd).c_str(), "topic ", 6))
            topic(cmd, server.getUserList()[socket], server.getChannelList());
        else if (!strncmp(utils.toLowerCase(cmd).c_str(), "invite ", 7))
            invite(cmd, server.getUserList()[socket], server.getUserList(), server.getChannelList());
        else if (!strncmp(utils.toLowerCase(cmd).c_str(), "quit ", 5) || utils.toLowerCase(cmd) == "quit")
        {
            quit(cmd, server.getUserList()[socket], server.getChannelList());
            return (1);
        }
        else
            usageCommand(cmd, server.getUserList()[socket]);
    }
    return(0);
}
