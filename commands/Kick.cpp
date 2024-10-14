#include "Commands.hpp"
#include "../server/Server.hpp"

std::string kickText(std::vector<std::string> reason, Server &server)
{
    Utils utils;
    std::string reason_tmp = "";
    std::map<int, User> userList = server.getUserList();
    std::vector <std::string> args;
    std::vector <std::string> tmp;
    std::vector <std::string> tofindchan;
    std::vector <std::string> tofinduser;
    bool checkchan = false;
    bool checkuser = false;
    for (size_t i = 0; i < reason.size(); i++)
    {
        if (findComma(reason[i]))
        {
            removeColon(&reason[i]);
            tmp = utils.split(reason[i], ',');
            if (checkchan == true)
                checkuser = true;
            for (size_t j = 0; j < tmp.size(); j++)
            {
                if (checkuser == true)
                    tofinduser.push_back(tmp[j]);
                if (checkchan == false)
                {
                    tofindchan.push_back(tmp[j]);
                    if (j == tmp.size() - 1)
                        checkchan = true;
                }
                args.push_back(tmp[j]);
            }
        }
        else
        {
            removeColon(&reason[i]);
            args.push_back(reason[i]);
        }
    }
    for (size_t i = 0; i < args.size(); i++)
        removeColon(&args[i]);
    size_t i = 1;
    while (i < args.size())
    {
        if (checkchan == true)
        {
            for (size_t j = 0; j < tofindchan.size(); j++)
                i++;
        }
        else
            i++;
        if (checkuser == true)
        {
            for (size_t j = 0; j < tofinduser.size(); j++)
                i++;
        }
        else
            i++;
        while (i < args.size())
        {
            reason_tmp += args[i] + " ";
            i++;
        }
    }
    reason.erase(reason.begin() + 1, reason.end() - 1);
    return (reason_tmp);
}

bool    SearchOperator(User &user, Channel &channel)
{
    std::map<int, User>::iterator it = channel.getUserOperators().begin();
    while (it != channel.getUserOperators().end())
    {
        if (it->first == user.getSocket())
            return (false);
        it++;
    }
    return (true);
}

void removeDouble(std::vector<std::string> *args)
{
    Utils utils;
    std::string s1;
    std::string s2;
    std::vector<std::string> tmp;

    if ((*args)[1] == (*args)[2])
    {
        args->erase(args->begin() + 1);
        return ;
    }
    if ((*args)[1][0] == '#' && (*args)[2][0] == '#')
    {
        args->erase(args->begin() + 1);
        return ;
    }
    for (size_t i = 0; i < args->size(); i++)
    {
        if (findComma((*args)[i]))
        {
            std::vector<std::string> tmp = utils.split((*args)[i], ',');
            for (size_t j = 0; j < tmp.size(); j++)
            {
                s1 = tmp[j];
                s2 = (*args)[1];
                if (s1 == s2)
                {
                    args->erase(args->begin() + 1);
                    return ;
                }
            }
                
        }
    }
}

void   removeColon(std::string *str)
{
    if ((*str)[0] == ':')
        *str = str->substr(1);
}

bool findComma(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ',')
            return (true);
    }
    return (false);
}

bool doubleComma(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ',')
        {
            if (str[i + 1] == ',')
                return (true);
        }
    }
    return (false);
}

bool doubleHash(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '#')
        {
            if (str[i + 1] == '#')
                return (true);
        }
    }
    return (false);
}

void    kick(std::string cmd, User &user, std::map<std::string, Channel>&channelList, Server &server)
{
    Utils utils;

    std::vector<std::string> args;
    std::vector<std::string> channelName;
    std::vector<std::string> nickname;
    std::string reason = "";

    args = utils.split(cmd, ' ');

    if (args.size() < 3)
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), args[0]));
    removeDouble(&args);
    if (doubleComma(args[1]) || doubleHash(args[1]))
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), args[0]));
    channelName = utils.split(args[1], ',');
    for (size_t i = 0; i < channelName.size(); i++)
        removeColon(&channelName[i]);
    if (channelName.size() == 0)
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), args[0]));
    if (doubleComma(args[2]) || doubleHash(args[2]))
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), args[0]));
    nickname = utils.split(args[2], ',');
    for (size_t i = 0; i < nickname.size(); i++)
        removeColon(&nickname[i]);
    if (args.size() > 2)
        reason = kickText(args, server);
    for (size_t i = 0; i < channelName.size(); i++)
    {
        if (channelName[i][0] != '#')
        {
            ERR_NOSUCHCHANNEL(user.getSocket(), user.getNickname(), channelName[i]);
            continue;
        }
        std::map<std::string, Channel>::iterator itChan = channelList.find(channelName[i]);
        if (itChan == channelList.end())
        {
            ERR_NOSUCHCHANNEL(user.getSocket(), user.getNickname(), channelName[i]);
            continue;
        }
        for (size_t i = 0; i < nickname.size(); i++)
        {
            if (nickname[i] == user.getNickname())
            {
                ERR_KICKME(user.getSocket(), user.getNickname(), channelName[i]);
                continue;
            }
            if (SearchOperator(user, itChan->second))
                return (ERR_CHANOPRIVSNEEDED(user.getSocket(), user.getNickname(), itChan->second.getName()));
            std::map<int, User>::iterator itUsers = itChan->second.getUsers().begin();
            bool check = false;
            while (itUsers != itChan->second.getUsers().end())
            {
                if (itUsers->second.getNickname() == nickname[i])
                {
                    std::string send_msg = ":" + user.getNickname() + " KICK " + itChan->second.getName() + " " + nickname[i];
                    if (reason != "")
                        send_msg += " :" + reason;
                    sendMsg(send_msg, itUsers->second.getSocket());
                    itChan->second.removeUser(itUsers->second.getSocket());
                    check = true;
                    break;
                }
                itUsers++;
            }
            if (check == false)
            {
                ERR_NOSUCHNICK(user.getSocket(), nickname[i]);
                continue;
            }
            std::string send_msg = ":" + user.getNickname() + " KICK " + itChan->second.getName() + " " + nickname[i];
            if (reason != "")
                    send_msg += " :" + reason;
            sendMsgAll(itChan->second.getUsers(), send_msg, "");
        }
    }
}