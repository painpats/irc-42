#include "server/Server.hpp"
#include "error/Error.hpp"


void handleSignal(int signal)
{
    (void)signal;
    throw Server::ServerException();
}

int main(int ac, char **av)
{
    if(ac != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    else
    {
        Utils utils;
        if (utils.validInput(av[1], av[2]) == false)
            return (1);
        Server serv(atoi(av[1]), av[2]);
        try
        {
            signal(SIGINT, handleSignal);
            serv.serverHandler();
        }
        catch(std::exception& e)
        {
            std::cerr << RED << e.what() << RESET <<std::endl;
        }
    }
    return (0);
}