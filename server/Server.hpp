#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sstream>
#include <string>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>
#include <poll.h>
#include <errno.h>
#include <map>
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define ORANGE "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

#include "../user/User.hpp"
#include "../channel/Channel.hpp"

class User;

class Server
{
    private:
        struct sockaddr_in servAddress;
        std::vector<struct pollfd> fds;
        int serverFd;
        int port;
        int optv;
        int addressLen;
        std::string password;
        std::map<int, User>	clientList;
        std::map<std::string, Channel> channelList;
        int     createSocket();
        void    serverBind();
        void    serverListen();
        int     serverAccept();
        void    serverInitUser(int client_fd);
        int     serverRequest(int server_fd);
        void    quitServer(int socket);

    public:
        Server(int port, std::string password);
        ~Server();
        void setServerFd(int serverFd);
        void serverHandler();
        std::string                     getPassword();
        std::map<int, User>&            getUserList();
        std::map<std::string, Channel>& getChannelList();

        class ServerException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif