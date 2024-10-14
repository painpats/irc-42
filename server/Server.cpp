#include "Server.hpp"
#include "../commands/Commands.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CONSTRUCTOR-DESTRUCTOR-OPERATOR ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

Server::Server(int port, std::string password)
{
    this->serverFd = -1;
    this->port = port;
    this->password = password;
    this->optv = 1;
    this->servAddress.sin_family = AF_INET;
    this->servAddress.sin_port = htons(this->port);
    this->servAddress.sin_addr.s_addr = INADDR_ANY;
    this->addressLen = sizeof(this->servAddress);
}

Server::~Server()
{
    for (std::vector<struct pollfd>::iterator it = this->fds.begin(); it != this->fds.end(); it++)
        close(it->fd);
    if (!this->fds.empty())
        this->fds.clear();
    if (this->serverFd != -1)
        close(this->serverFd);
    for (std::map<int, User>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
        close(it->first);
    if (!this->clientList.empty())
        this->clientList.clear();
    if (!this->channelList.empty())
        this->channelList.clear();
}

const char *Server::ServerException::what() const throw()
{
    return ("Server has been stoped");
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// GET-SET /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void Server::setServerFd(int serverFd)
{
    this->serverFd = serverFd;
}

std::string Server::getPassword()
{
    return (this->password);
}

std::map<int, User>	&Server::getUserList()
{
	return (this->clientList);
}

std::map<std::string, Channel>	&Server::getChannelList()
{
	return (this->channelList);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// FUNC ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void Server::quitServer(int socket)
{
    leaveAllChannels(this->clientList[socket], this->channelList);
    this->clientList.erase(socket);
}

int Server::createSocket()
{
    int protocol = 0;
    int server_fd = socket(AF_INET, SOCK_STREAM, protocol);
    if (server_fd == -1)
        throw ServerException();
    else
    {
        std::cout << GREEN << "Server created with port: [" << this->port << "] and password: [" << this->password << "]" << RESET << std::endl;
        return server_fd;
    }
}

void Server::serverBind()
{
    int bind_status = bind(this->serverFd, (struct sockaddr *)&this->servAddress, sizeof(this->servAddress));
    if (bind_status == -1)
    {
        std::cerr << RED << "Bind failed" << RESET << std::endl;
        std::cerr << RED << "Errno : " << strerror(errno) << RESET << std::endl;
        throw ServerException();
    }
    else
        std::cout << GREEN << "Bind successful" << RESET << std::endl;
}

void Server::serverListen()
{
    int listen_status = listen(this->serverFd, 3);
    if (listen_status == -1)
        throw ServerException();
    else
        std::cout << GREEN << "Server listening" << RESET << std::endl;
}

int Server::serverAccept()
{
    int socket = accept(this->serverFd, (struct sockaddr *)&this->servAddress, (socklen_t *)&this->addressLen);
    if (socket == -1)
        throw ServerException();
    else
    {
        std::cout << GREEN << "Server accepted connection" << RESET << std::endl;
        return socket;
    }
}

void Server::serverInitUser(int socket)
{
    this->clientList[socket] = User();
    this->clientList[socket].setSocket(socket);
    this->clientList[socket].setMessages(0);
}

int Server::serverRequest(int socket)
{
    std::string cmd;
    char reveive[1024];
    ssize_t bytes;
    while (true)
    {
        bytes = recv(socket, reveive, sizeof(reveive), 0);
        if (bytes == -1)
        {
            std::cerr << RED << "Read failed" << RESET << std::endl;
            quitServer(socket);
            return -1;
        }
        else if (bytes == 0)
        {
            std::cerr << RED << "Client with nickname " << this->clientList[socket].getNickname() << " disconnected" << RESET << std::endl;
            quitServer(socket);
            return (-1);
        }
        cmd.append(reveive, bytes);
        if (cmd.find('\n') != std::string::npos)
        {
            std::cout << ORANGE << "Server received: " << cmd << RESET;
            if (handlerCommand(cmd.c_str(), socket, *this))
            {
                std::cerr << RED << "Client with nickname " << this->clientList[socket].getNickname() << " disconnected" << RESET << std::endl;
                quitServer(socket);
                return (-1);
            }
            return (0);
        }
    }
}


void Server::serverHandler()
{
    int fdServ = createSocket();
    setServerFd(fdServ);
    serverBind();
    serverListen();

    std::vector<struct pollfd> fds;
    struct pollfd fdsStruct = {this->serverFd, POLLIN, 0};
    fds.push_back(fdsStruct);

    while (1)
    {
        if (poll(&fds[0], fds.size(), -1) < 0)
        {
            perror("Error poll");
            throw ServerException();
        }
        for (size_t i = 0; i < fds.size(); ++i)
        {
            if (fds[i].revents & POLLIN)
            {
                if (i == 0)
                {
                    int socket = serverAccept();
                    serverInitUser(socket);

                    struct pollfd newClient = {socket, POLLIN, 0};
                    fds.push_back(newClient);
                    sendMsg("Register commands:\nPASS: <password>\nNICK: <nickname>\nUSER: <username>", socket);
                }
                else
                {
                    if (serverRequest(fds[i].fd) == -1)
                    {
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i;
                    }
                }
            }
        }
    }
}
