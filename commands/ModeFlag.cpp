#include "Commands.hpp"
#include "../utils/Utils.hpp"

bool doubleFlag(std::multimap<char, char> allFlags, char flag, int pos)
{
    int i = 0;
    for (std::multimap<char, char>::iterator itflag = allFlags.begin(); itflag != allFlags.end(); itflag++)
    {
        if (pos == i)
        {
            i++;
            continue;
        }
        if (itflag->second == flag)
            return true;
        i++;
    }
    return false;
}

bool unknownFlags(std::string flags)
{
    int i = 0;
    if (flags[0] != '+' && flags[0] != '-')
        return false;
    while (flags[i])
    {
        if (flags[i] == '+' || flags[i] == '-')
        {
            i++;
            if (flags[i] && (flags[i] == '+' || flags[i] == '-'))
                return (false);
            while(flags[i])
            {
                if (flags[i] == '+' || flags[i] == '-')
                    break ;
                if (flags[i] && !isalpha(flags[i]))
                    return (false);
                i++;
            }
        }
        else
            i++;
    }
    if (!isalpha(flags[flags.length() - 1]))
        return (false);
    return true;
}


std::multimap <char, char> fillFlags(std::string flags)
{
    int i = 0;
    std::multimap<char, char> allFlags;
    char sign;
    while (flags[i])
    {
        if (flags[i] == '+' || flags[i] == '-')
        {
            sign = flags[i];
            i++;
            while(flags[i])
            {
                if (flags[i] == '+' || flags[i] == '-')
                    break ;
                if (flags[i] == '\0')
                    return allFlags;
                allFlags.insert(std::make_pair(sign, flags[i]));
                i++;
            }
        }
        else
            i++;
    }
    return allFlags;
}

bool    argIsNick(std::string nickname, std::map <int, User> users)
{
    std::map <int, User>::iterator itUser = users.begin();
    while (itUser != users.end())
    {
        if (itUser->second.getNickname() == nickname)
            return true;
        itUser++;
    }
    return false;
}

void    sortFlags(std::string *password, std::list<std::string> *nickname, int *nbr, std::vector <std::string> cmd, std::map <int, User> users)
{
    bool check = false;
    std::map <int, User>::iterator itUser = users.begin();
    Utils utils;
    size_t i = 3;
    bool checknbr = false;
    while (i < cmd.size())
    {
        while (itUser != users.end())
        {
            if (itUser->second.getNickname() == cmd[i])
            {
                check = false;
                if (nickname->empty())
                {
                    nickname->push_back(cmd[i]);
                    break;
                }
                else
                {
                    for (std::list<std::string>::iterator itNickname = nickname->begin(); itNickname != nickname->end(); itNickname++)
                    {
                        if (*itNickname == cmd[i])
                        {
                            if (*password != "")
                            {
                                check = true;
                                break;
                            }
                            *password = cmd[i];
                            check = true;
                        }
                    }
                }
                if (check == true)
                    break;
                nickname->push_back(cmd[i]);
                break;
            }
            itUser++;
        }
        itUser = users.begin();
        if (utils.IsAllDigits(cmd[i]))
        {
            if (checknbr == true)
                *password = cmd[i];
            else
                *nbr = atoi(cmd[i].c_str());
            checknbr = true;
        }
        if (argIsNick(cmd[i], users) == false && utils.IsAllDigits(cmd[i]) == false)
        {
            if (*password == "")
                *password = cmd[i];
            else
                nickname->push_back(cmd[i]);
        }
        i++;
    }
}