#include "Commands.hpp"

void    sendMsg(std::string msg, int socket)
{
    msg += "\r\n";
    send(socket, msg.c_str(), msg.size(), 0);
}

void sendMsgAll(std::map<int, User> users, std::string msg, std::string notcall)
{
    std::map<int, User>::iterator it = users.begin();
    while (it != users.end())
    {
        if (it->second.getNickname() == notcall)
        {
            it++;
            continue;
        }
        sendMsg(msg, it->second.getSocket());
        it++;
    }
}

std::string nameReply(Channel &channel, User &user)
{
    std::map<int, User>::iterator itUser = channel.getUsers().begin();
    std::string send_msg = ": 353 " + user.getNickname() + " = " + channel.getName() + " :";
    while (itUser != channel.getUsers().end())
    {
        if (channel.getUserOperators().find(itUser->second.getSocket()) != channel.getUserOperators().end())
            send_msg += "@";
        else
            send_msg += "+";
        send_msg += itUser->second.getNickname();
        send_msg += " ";
        itUser++;
    }
    return (send_msg);
}
