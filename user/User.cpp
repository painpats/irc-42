#include "User.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CONSTRUCTOR-DESTRUCTOR-OPERATOR ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

User::User() {
    this->_socket = -1;
    this->_messages = 0;
    this->_register = false;
}

User::~User() {
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// GET-SET /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

std::string& User::getNickname() {
    return this->_nickname;
}

std::string& User::getUsername() {
    return this->_username;
}

std::string& User::getPassword() {
    return this->_password;
}

bool User::getRegister() {
    return this->_register;
}

int User::getMessages() {
    return this->_messages;
}

int User::getSocket() {
    return this->_socket;
}

void User::setNickname(std::string nickname) {
    this->_nickname = nickname;
}

void User::setUsername(std::string username) {
    this->_username = username;
}

void User::setPassword(std::string password) {
    this->_password = password;
}

void User::setRegister() {
    this->_register = true;
}

void User::setSocket(int socket) {
    this->_socket = socket;
}

void User::setMessages(int messages) {
    this->_messages = messages;
}

