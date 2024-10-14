#ifndef CMD_HPP
#define CMD_HPP

#include "../server/Server.hpp"
#include "../user/User.hpp"
#include "../channel/Channel.hpp"
#include "../utils/Utils.hpp"
#include "../error/Error.hpp"

int    handlerCommand(std::string buffer, int socket, Server &server);
void   quit(std::string sstr, User &user, std::map<std::string, Channel>&channelList);

////////////////////////////////////////// REGISTER /////////////////////////////////////////////////
void    registerPassword(std::vector<std::string> sstr, Server &server, int socket);
void    registerNickname(std::vector<std::string> sstr, Server &server, int socket);
void    registerUsername(std::vector<std::string> sstr, Server &server, int socket);
void    hexchatConnection(Server &server, int socket, std::vector<std::string> sstr);
void    checkRegister(Server &server, int socket, std::vector<std::string> sstr);

////////////////////////////////////////// JOIN ////////////////////////////////////////////////
void    leaveAllChannels(User &user, std::map<std::string, Channel>&channelList);
int     checkChannelMode(User &user, Channel &channel, std::vector<std::string> password, size_t i);
void    createChannel(User &user, Channel &channel, std::string channelName);
void    joinChannel(User &user, Channel &channel, std::vector<std::string> password, int i);
void    join(std::string cmd, User &user, std::map<std::string, Channel>&channelList);

////////////////////////////////////////// INVITE //////////////////////////////////////////////
void    invite(std::string cmd, User &user, std::map<int, User>&userList, std::map<std::string, Channel>&channelList);

////////////////////////////////////////// PRIVMSG /////////////////////////////////////////////
void    privmsg(std::string cmd, User &user, std::map<int, User>&userlist, std::map<std::string, Channel>&channelList);

////////////////////////////////////////// TOPIC ///////////////////////////////////////////////
void    topic(std::string cmd, User &user, std::map<std::string, Channel>&channelList);

////////////////////////////////////////// MODE ////////////////////////////////////////////////
std::string                 getModes(Channel &channel);
void                        modeLimitSet(Channel &channel, User &user, int limitUsers);
void                        modeLimitUnset(Channel &channel);
void                        modeTopicSet(Channel &channel);
void                        modeTopicUnset(Channel &channel);
void                        modeInviteSet(Channel &channel);
void                        modeInviteUnset(Channel &channel);
void                        modeOperatorUnset(Channel &channel, std::list<std::string> Nickname, User &user, std::string Password);
void                        modeOperatorSet(Channel &channel, std::list<std::string> Nickname, User &user, std::string Password);
void                        modePassSet(Channel &channel, User &user, std::string Password);
void                        modePassUnset(Channel &channel, User &user, std::string Password);
bool                        doubleFlag(std::multimap<char, char> allFlags, char flag, int pos);
void                        changeModes(std::multimap<char, char> allFlags, std::string Password, std::list<std::string> Nickname, int limitUsers, Channel &channel, User &user);
bool                        unknownFlags(std::string flags);
std::multimap<char, char>   fillFlags(std::string flags);
bool                        argIsNick(std::string nickname, std::map <int, User> users);
void                        sortFlags(std::string *password, std::list<std::string> *nickname, int *nbr, std::vector <std::string> cmd, std::map <int, User> users);
void                        printMode(std::vector<std::string>cmd, User &user, std::map<std::string, Channel>&channelList);
void                        mode(std::string cmd, User &user, std::map<std::string, Channel>&channelList);

////////////////////////////////////////// KICK ////////////////////////////////////////////////
std::string kickText(std::vector<std::string> reason, Server &server);
bool        SearchOperator(User &user, Channel &channel);
void        removeColon(std::string *str);
void        removeDouble(std::vector<std::string> *args);
bool        findComma(std::string str);
bool        doubleComma(std::string str);
bool        doubleHash(std::string str);
void        kick(std::string cmd, User &user, std::map<std::string, Channel>&channelList, Server &server);

////////////////////////////////////////// PART ////////////////////////////////////////////////
void        part(std::string cmd, User &user, std::map<std::string, Channel>&channelList);

////////////////////////////////////////// SEND ////////////////////////////////////////////////
void        sendMsg(std::string msg, int socket);
void        sendMsgAll(std::map<int, User> users, std::string msg, std::string notcall);
std::string nameReply(Channel &channel, User &user);

#endif