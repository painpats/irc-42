#include "Commands.hpp"

// std::string quitText(std::string cmd)
// {
//     std::string text = "";

//     for (size_t i = 0; cmd[i]; i++)
//     {
//         if (cmd[i] == ':')
//         {
//             i++;
//             if (cmd[i] == ':')
//             i++;
//             while (cmd[i])
//             {
//                 text += cmd[i];
//                 i++;
//             }
//         }
//     }
//     return (text);
// }

// std::string quitText(std::string cmd)
// {
//     std::string text = "";

//     int i = 0;
//     while (cmd[i])
//     {
//         if (cmd[i] == ':')
//         {
//             i++;
//             if (cmd[i] == ':')
//             i++;
//             while (cmd[i])
//             {
//                 text += cmd[i];
//                 i++;
//             }
//         }
//         else
//             i++;
//     }
//     return (text);
// }

std::string    quitText(std::string cmd)
{
    int space = 0;
    int i = 0;
    std::string text;
    while (cmd[i])
    {
        if (cmd[i] == 32 && space < 1)
        {
            space += 1;
            while (cmd[i] && cmd[i] == 32)
                i++;
        }
        else if (space == 1)
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

void   quit(std::string cmd, User &user, std::map<std::string, Channel>&channelList)
{
    Utils utils;

    leaveAllChannels(user, channelList);
    if (utils.toLowerCase(cmd) == "quit")
    {
        std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + " QUIT :Leaving";
        sendMsg(msg, user.getSocket());
    }
    else
    {
        std::string reason = quitText(cmd);
        std::string msg = ":" + user.getNickname() + "!" + user.getUsername() + " QUIT :" + reason;
        sendMsg(msg, user.getSocket());
    }
}