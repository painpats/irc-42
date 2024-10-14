#include "Channel.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CONSTRUCTOR-DESTRUCTOR-OPERATOR ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

Channel::Channel()
{
    this->modeInvite = false;
    this->modeKey = false;
    this->modeTopic = false;
    this->modeLimit = false;
}

Channel::Channel(std::string channelName): name(channelName)
{
    this->modeInvite = false;
    this->modeKey = false;
    this->modeTopic = false;
    this->modeLimit = false;
}

Channel::~Channel() {}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// GET-SET /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void Channel::setName(std::string name)
{
    this->name = name;
}

void Channel::setPassword(std::string password)
{
    this->password = password;
}

void Channel::setUserLimit(int userLimit)
{
    this->userLimit = userLimit;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

void Channel::setModeInvite(bool perm)
{
    this->modeInvite = perm;
}

void Channel::setModeKey(bool perm)
{
    this->modeKey = perm;

}

void Channel::setModeTopic(bool perm)
{
    this->modeTopic = perm;

}

void Channel::setModeLimit(bool perm)
{
    this->modeLimit = perm;

}

///////////////////////////////////////////////////////////////////////////////////////////////

std::string Channel::getName()
{
    return (this->name);
}

std::string  Channel::getPassword()
{
    return(this->password);
}

 int Channel::getUserLimit()
{
    return (this->userLimit);
}

int  Channel::getNbUser()
{
    return (this->users.size());
}

std::string  Channel::getTopic()
{
    return (this->topic);
}

bool Channel::getModeInvite()
{
    return (this->modeInvite);
}

bool Channel::getModeKey()
{
    return (this->modeKey);
}

bool Channel::getModeLimit()
{
    return (this->modeLimit);
}

bool Channel::getModeTopic()
{
    return (this->modeTopic);
}

std::map<int, User>& Channel::getUsers()
{
    return this->users;
}

std::map<int, User>& Channel::getUserOperators()
{
    return (this->userOperators);
}

std::map<int, User>& Channel::getInvitations()
{
    return (this->invitations);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// FUNC ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void Channel::addUser(int socket, User& user)
{
    this->users[socket] = user;
}

void Channel::addOperator(int socket, User& user)
{
    this->userOperators[socket] = user;
}

void Channel::removeOperator(int socket)
{
    this->userOperators.erase(socket);
}

void Channel::addInvitation(int socket, User &user)
{
    this->invitations[socket] = user;
}

void Channel::removeUser(int socket)
{
    this->users.erase(socket);
    this->userOperators.erase(socket);
}

void Channel::removeInvitation(int socket)
{
    this->invitations.erase(socket);
}