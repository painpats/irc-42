/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slepetit <slepetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:30:12 by slepetit          #+#    #+#             */
/*   Updated: 2024/04/23 19:28:52 by slepetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
#define ERROR_HPP
#include "../commands/Commands.hpp"
#include "../utils/Utils.hpp"

#define ERR_NOUSERNAMEGIVEN(fd)		    	                sendMsg("No username given", fd)
#define ERR_ERRONEUSUSERNAME(fd, client, username)	        sendMsg(client + " " + username + " :Erroneus username", fd)
#define ERR_KICKME(fd, client, channel)                     sendMsg(client + " " + channel + " :Cannot kick yourself in this server", fd)
#define RPL_WELCOME(fd, client)                             sendMsg(": 001 " + client + " :Welcome to Internet Relay Network " + client + "!", fd)
#define RPL_CHANNELMODEIS(fd, client, channel, modes)       sendMsg(": 324 " + client + " " + channel + " " + modes, fd)
#define RPL_NOTOPIC(fd, client, channel)                    sendMsg(": 331 " + client + " " + channel + " :No topic is set", fd)
#define RPL_TOPIC(fd, client, channel, topic)               sendMsg(": 332 " + client + " " + channel + " :" + topic + "", fd)
#define RPL_INVITING(fd, client, guest, channel)            sendMsg(": 341 " + client + " " + guest + " " + channel, fd)
#define ERR_NOSUCHNICK(fd, client)					        sendMsg(": 401 " + client + " :No such nick/channel", fd)
#define ERR_NOSUCHCHANNEL(fd, client, channel)              sendMsg(": 403 " + client + " " + channel + " :No such channel", fd)
#define ERR_CANNOTSENDTOCHAN(fd, client, channel)           sendMsg(": 404 " + client + " " + channel + " :Cannot send to channel", fd)
#define ERR_NOTEXTTOSEND(fd, client)                        sendMsg(": 412 " + client + " :No text to send", fd)
#define ERR_NONICKNAMEGIVEN(fd, client)                     sendMsg(": 431 " + client + " :No nickname given", fd)
#define ERR_ERRONEUSNICKNAME(fd, client, nick)              sendMsg(": 432 " + client + " " + nick + " :Erroneus nickname", fd)
#define ERR_NICKNAMEINUSE(fd, client, nick)                 sendMsg(": 433 " + client + " " + nick + " :Nickname is already in use", fd)
#define ERR_USERNOTINCHANNEL(fd, client, target, channel)   sendMsg(": 441 " + client + " " + target + " " + channel + " :They aren't on that channel", fd)
#define ERR_NOTONCHANNEL(fd, client, channel)               sendMsg(": 442 " + client + " " + channel + " :You're not on that channel", fd)
#define ERR_USERONCHANNEL(fd, client, pseudo, channel)      sendMsg(": 443 " + client + " " + pseudo + " " + channel + " :is already on channel", fd)
#define ERR_NEEDMOREPARAMS(fd, client, cmd)                 sendMsg(": 461 " + client + " " + cmd + " :Not enough parameters", fd)
#define ERR_ALREADYREGISTRED(fd, client)                    sendMsg(": 462 " + client + " :You may not reregister", fd)
#define ERR_PASSWDMISMATCH(fd, client)                      sendMsg(": 464 " + client + ": Password incorrect", fd)
#define ERR_KEYSET(fd, client, channel)                     sendMsg(": 467 " + client + " " + channel + " :Channel key already set", fd)
#define ERR_CHANNELISFULL(fd, client, channel)              sendMsg(": 471 " + client + " " + channel + " :Cannot join channel (+l)", fd)
#define ERR_INVITEONLYCHAN(fd, client, channel)             sendMsg(": 473 " + client + " " + channel + " :Cannot join channel (+i)", fd)
#define ERR_BADCHANNELKEY(fd, client, channel)              sendMsg(": 475 " + client + " " + channel + " :Cannot join channel (+k)", fd)
#define ERR_NOCHANMODES(fd, client, channel)                sendMsg(": 477 " + client + " " + channel + " :Channel doesn't support modes", fd)
#define ERR_BADCHANNELNAME(fd, client, channel)             sendMsg(": 479 " + client + " " + channel + " :Invalid channel name", fd)
#define ERR_CHANOPRIVSNEEDED(fd, client, channel)           sendMsg(": 482 " + client + " " + channel + " :You're not channel operator", fd)
#define ERR_UNKNOWNCOMMAND(fd, client, command)             sendMsg(": 421 " + client + " " + command + " :Unknown command", fd)
#define ERR_UNKNOWNMODE(fd, client, flag, channel)          sendMsg(": 472 " + client + " " + flag + ": is unknown mode char to me for " + channel, fd)

#endif
