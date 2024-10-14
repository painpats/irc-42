#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <algorithm>
#include <string>
#include "../user/User.hpp"

class Channel
{
    private:
        std::string         name;
        std::string         password;
        std::string         topic;
        int                 userLimit;
        bool                modeInvite;
        bool                modeLimit;
		bool				modeTopic;
		bool				modeKey;
        std::map<int, User> users;
        std::map<int, User> userOperators;
        std::map<int, User> invitations;

    public:
        Channel();
        Channel(std::string channelName);
        ~Channel();
        // SET-VAR
        void setName(std::string name);
        void setPassword(std::string password);
        void setTopic(std::string topic);
        void setUserLimit(int userLimit);
        // SET-ACTIVE-MODE
		void setModeInvite(bool perm);
		void setModeLimit(bool perm);
		void setModeTopic(bool perm);
		void setModeKey(bool perm);
        // GET-VAR
        std::string         getName();
        std::string         getPassword();
        std::string         getTopic();
        int                 getNbUser();
        int                 getUserLimit();
        // GET-ACTIVE-MODE
		bool				getModeInvite();
		bool				getModeLimit();
		bool				getModeTopic();
		bool				getModeKey();
        // GET-MAP
        std::map<int, User> &getUsers();
        std::map<int, User> &getUserOperators();
        std::map<int, User> &getInvitations();
        // FUNC
        void addInvitation(int socket, User &user);
        void addUser(int socket, User& user);
        void addOperator(int socket, User &user);
        void removeOperator(int socket);
        void removeUser(int socket);
        void removeInvitation(int socket);
};

#endif
