#include "Commands.hpp"
#include "../server/Server.hpp"
#include "../error/Error.hpp"

#define FLAG_O 'o'
#define FLAG_K 'k'
#define FLAG_L 'l'
#define FLAG_T 't'
#define FLAG_I 'i'
#define NEG '-'
#define POS '+'

void modeLimitSet(Channel &channel, User &user, int limitUsers)
{
    (void)user;
    if (limitUsers <= 0 || limitUsers > 10000)
        return ;
    channel.setModeLimit(true);
    channel.setUserLimit(limitUsers);

}

void modeLimitUnset(Channel &channel)
{
    channel.setModeLimit(false);
    channel.setUserLimit(0);
}

void modeTopicSet(Channel &channel)
{
    channel.setModeTopic(true);
}

void modeTopicUnset(Channel &channel)
{
    channel.setModeTopic(false);
}

void modeInviteSet(Channel &channel)
{
    channel.setModeInvite(true);
}

void modeInviteUnset(Channel &channel)
{
    channel.setModeInvite(false);
}

void modePassSet(Channel &channel, User &user, std::string Password)
{
    if (Password == "")
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("MODE")));
    if (channel.getPassword() != "")
        return (ERR_KEYSET(user.getSocket(), user.getNickname(), channel.getName()));
    if (Password.length() > 20)
        return ;
    channel.setPassword(Password);
    channel.setModeKey(true);

}

void modePassUnset(Channel &channel, User &user, std::string Password)
{
    if (channel.getPassword().empty())
        return ;
    if (Password == "")
        return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("MODE")));
    if (channel.getPassword() != Password)
        return (ERR_PASSWDMISMATCH(user.getSocket(), user.getNickname()));
    channel.setPassword("");
    channel.setModeKey(false);
}

void modeOperatorUnset(Channel &channel, std::list<std::string> Nickname, User &user, std::string Password)
{
    if (Nickname.empty())
    {
        if (Password == "")
            return(ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("MODE")));
        else
            return (ERR_USERNOTINCHANNEL(user.getSocket(), user.getNickname(), std::string(""), channel.getName()));
    }
    std::list<std::string>::iterator itNickname = Nickname.begin();
    bool check = false;
    while (itNickname != Nickname.end())
    {
        std::map<int, User>::iterator itUser = channel.getUsers().begin();
        check = false;
        while (itUser != channel.getUsers().end())
        {
            if (itUser->second.getNickname() == *itNickname)
            {
                channel.removeOperator(itUser->first);

                check = true;
                break;
            }
            itUser++;
        }
        if (check == false)
            ERR_USERNOTINCHANNEL(user.getSocket(), user.getNickname(), *itNickname, channel.getName());
        itNickname++;
    }
    sendMsgAll(channel.getUsers(), nameReply(channel, user), "");
}

void modeOperatorSet(Channel &channel, std::list<std::string> Nickname, User &user, std::string Password)
{
    if (Nickname.empty())
    {
        if (Password == "")
            return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("MODE")));
        else
            return (ERR_USERNOTINCHANNEL(user.getSocket(), user.getNickname(), std::string(""), channel.getName()));
    }
    std::list<std::string>::iterator itNickname = Nickname.begin();
    bool check = false;
    while (itNickname != Nickname.end())
    {
        std::map<int, User>::iterator itUser = channel.getUsers().begin();
        check = false;
        while (itUser != channel.getUsers().end())
        {
            if (itUser->second.getNickname() == *itNickname)
            {
                channel.addOperator(itUser->first, itUser->second);

                check = true;
                break;
            }
            itUser++;
        }
        if (check == false)
            ERR_USERNOTINCHANNEL(user.getSocket(), user.getNickname(), *itNickname, channel.getName());
        itNickname++;
    }
    sendMsgAll(channel.getUsers(), nameReply(channel, user), "");
}


void changeModes(std::multimap<char, char> allFlags, std::string Password, std::list<std::string> Nickname, int limitUsers, Channel &channel, User &user)
{
    (void)channel;
    (void)Nickname;
    int pos = 0;
    for (std::multimap<char, char>::iterator itflag = allFlags.begin(); itflag != allFlags.end(); itflag++)
    {
        if (itflag->second == FLAG_O && itflag->first == POS && doubleFlag(allFlags, FLAG_O, pos) == false)
            modeOperatorSet(channel, Nickname, user, Password);
        else if (itflag->second == FLAG_O && itflag->first == NEG && doubleFlag(allFlags, FLAG_O, pos) == false)
            modeOperatorUnset(channel, Nickname, user, Password);
        else if (itflag->second == FLAG_K && itflag->first == POS && doubleFlag(allFlags, FLAG_K, pos) == false)
        {
            if (Password == "")
                return (ERR_NEEDMOREPARAMS(user.getSocket(), user.getNickname(), std::string("MODE")));
            modePassSet(channel, user, Password);
        }
        else if (itflag->second == FLAG_K && itflag->first == NEG && doubleFlag(allFlags, FLAG_K, pos) == false)
            modePassUnset(channel, user, Password);
        else if (itflag->second == FLAG_L && itflag->first == POS && doubleFlag(allFlags, FLAG_L, pos) == false)
            modeLimitSet(channel, user, limitUsers);
        else if (itflag->second == FLAG_L && itflag->first == NEG && doubleFlag(allFlags, FLAG_L, pos) == false)
            modeLimitUnset(channel);
        else if (itflag->second == FLAG_T && itflag->first == POS && doubleFlag(allFlags, FLAG_T, pos) == false)
            modeTopicSet(channel);
        else if (itflag->second == FLAG_T && itflag->first == NEG && doubleFlag(allFlags, FLAG_T, pos) == false)
            modeTopicUnset(channel);
        else if (itflag->second == FLAG_I && itflag->first == POS && doubleFlag(allFlags, FLAG_I, pos) == false)
            modeInviteSet(channel);
        else if (itflag->second == FLAG_I && itflag->first == NEG && doubleFlag(allFlags, FLAG_I, pos) == false)
            modeInviteUnset(channel);
        else
        {
            std::string flag = "";
            flag += itflag->second;
            ERR_UNKNOWNMODE(user.getSocket(), user.getNickname(), flag, channel.getName());
        }
        pos++;
    }
}