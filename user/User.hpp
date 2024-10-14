#ifndef USER_HPP
#define USER_HPP
#include <iostream>

class User
{
    private :
        std::string _nickname;
        std::string _username;
        std::string _password;
        bool        _register;
        int _socket;
        int _messages;

    public:
        User();
        ~User();
        std::string& getNickname();
        std::string& getUsername();
        std::string& getPassword();
        bool         getRegister();
        int          getMessages();
        int          getSocket();
        void setNickname(std::string nickname);
        void setUsername(std::string username);
        void setPassword(std::string password);
        void setRegister();
        void setSocket(int socket);
        void setMessages(int messages);
};

#endif