#include "Commands.hpp"

void    registerPassword(std::vector<std::string> sstr, Server &server, int socket)
{
    Utils utils;

    if (sstr.size() > 1)
        return ;
    sstr[0] = utils.removeCRLF(sstr[0]);
    std::vector<std::string> tmp = utils.split(sstr[0], ' ');
    if (utils.toLowerCase(tmp[0]) == "pass" || utils.toLowerCase(tmp[0]) == "pass ")
    {
        if (tmp.size() == 1)
            return(ERR_NEEDMOREPARAMS(socket, server.getUserList()[socket].getNickname() , std::string("PASS")));
        if (tmp.size() > 2)
            return(ERR_PASSWDMISMATCH(socket, server.getUserList()[socket].getNickname()));
        if (server.getPassword() == tmp[1])
            server.getUserList()[socket].setPassword(tmp[1]);
        else
            return(ERR_PASSWDMISMATCH(socket, server.getUserList()[socket].getNickname()));
    }
    else
        sendMsg("Command for password: PASS <password>", socket);
}

void    registerNickname(std::vector<std::string> sstr, Server &server, int socket)
{
    Utils utils;

    if (sstr.size() > 1)
        return ;
    sstr[0] = utils.removeCRLF(sstr[0]);
    std::vector<std::string> tmp = utils.split(sstr[0], ' ');
    if (utils.toLowerCase(tmp[0]) == "nick" || utils.toLowerCase(tmp[0]) == "nick ")
    {
        if (tmp.size() == 1)
            return(ERR_NONICKNAMEGIVEN(socket, server.getUserList()[socket].getNickname()));
        if (tmp.size() > 2)
            return(ERR_NONICKNAMEGIVEN(socket, server.getUserList()[socket].getNickname()));
        if (tmp[1].size() > 9)
            return(ERR_ERRONEUSNICKNAME(socket, server.getUserList()[socket].getNickname(), tmp[1]));
        if (utils.IsValidAlphaNum(tmp[1]) == false)
            return(ERR_ERRONEUSNICKNAME(socket, server.getUserList()[socket].getNickname(), tmp[1]));
        std::map<int, User>::iterator it = server.getUserList().begin();
        while (it != server.getUserList().end())
        {
            if (utils.toLowerCase(it->second.getNickname()) == utils.toLowerCase(tmp[1]))
                return (ERR_NICKNAMEINUSE(socket, server.getUserList()[socket].getNickname(), tmp[1]));
            it++;
        }
        server.getUserList()[socket].setNickname(tmp[1]);
    }
    else
        sendMsg("Command for nickname: NICK <nickname>", socket);
}

void    registerUsername(std::vector<std::string> sstr, Server &server, int socket)
{
    Utils utils;

    if (sstr.size() > 1)
        return ;
    sstr[0] = utils.removeCRLF(sstr[0]);
    std::vector<std::string> tmp = utils.split(sstr[0], ' ');
    if (utils.toLowerCase(tmp[0]) == "user" || utils.toLowerCase(tmp[0]) == "user ")
    {
        if (tmp.size() == 1)
            return(ERR_NEEDMOREPARAMS(socket, server.getUserList()[socket].getNickname(), std::string("USER")));
        if (tmp.size() > 2)
            return(ERR_NOUSERNAMEGIVEN(socket));
        if (tmp[1].size() > 9)
            return(ERR_ERRONEUSUSERNAME(socket, server.getUserList()[socket].getNickname(), tmp[1]));
        if (utils.IsValidAlphaNum(tmp[1]) == false)
            return(ERR_ERRONEUSUSERNAME(socket, server.getUserList()[socket].getNickname(), tmp[1]));
        server.getUserList()[socket].setUsername(tmp[1]);
        server.getUserList()[socket].setRegister();
        RPL_WELCOME(socket, server.getUserList()[socket].getNickname());
    }
    else
        sendMsg("Command for username: user <username>", socket);
}

void    hexchatConnection(Server &server, int socket, std::vector<std::string> sstr)
{
    Utils utils;

    if (sstr.size() != 4)
        return (sendMsg("Auto-connection failed.. Please register manually", socket));
    std::vector<std::string> tmp;
    tmp.push_back(sstr[1]);
    registerPassword(tmp, server, socket);
    tmp.clear();
    if (server.getUserList()[socket].getPassword().empty())
        return (sendMsg("Auto-connection failed.. Please register manually", socket));
    tmp.push_back(sstr[2]);
    registerNickname(tmp, server, socket);
    tmp.clear();
    if (server.getUserList()[socket].getNickname().empty())
        return (sendMsg("Auto-connection failed.. Please register manually", socket));
    std::vector<std::string> username = utils.split(sstr[3], 32);
    username[0] += " " + username[1];
    username.erase(username.begin()+1, username.end());
    registerUsername(username, server, socket);
    username.clear();
    if (server.getUserList()[socket].getUsername().empty())
        return (sendMsg("Auto-connection failed.. Please register manually", socket));
    server.getUserList()[socket].setRegister();
}

void    checkRegister(Server &server, int socket, std::vector<std::string> sstr)
{
    if (!strcmp(sstr[0].c_str(), "CAP LS 302\r"))
        hexchatConnection(server, socket, sstr);
    else if (server.getUserList()[socket].getPassword().empty())
        registerPassword(sstr, server, socket);
    else if (server.getUserList()[socket].getNickname().empty())
        registerNickname(sstr, server, socket);
    else if (server.getUserList()[socket].getUsername().empty())
        registerUsername(sstr, server, socket);
    if (server.getUserList()[socket].getRegister() == true)
        std::cout << GREEN << "Client with nickname " << server.getUserList()[socket].getNickname() <<  " is fully registered" << std::endl;
}