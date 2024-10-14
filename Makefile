NAME = ircserv

SRC	= 	main.cpp \
		utils/Utils.cpp \
		server/Server.cpp \
		user/User.cpp \
		channel/Channel.cpp \
		commands/Commands.cpp \
		commands/Register.cpp \
		commands/Send.cpp \
		commands/Join.cpp \
		commands/Invite.cpp \
		commands/Kick.cpp \
		commands/Part.cpp \
		commands/Mode.cpp \
		commands/ModeFlag.cpp \
		commands/ModeChanges.cpp \
		commands/Privmsg.cpp \
		commands/Topic.cpp \
		commands/Quit.cpp 

CC 	= c++

FLAGS = -std=c++98 -Wall -Wextra -Werror

OBJS = $(SRC:.cpp=.o)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

all: $(NAME) $(OBJS)

%.o: %.cpp
		$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re