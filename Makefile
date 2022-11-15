NAME		= ft_irc
SRCSDIR		= .
OBJSDIR		= objs
INCLUDES	= .
SRCS		= srcs/main.cpp \
				srcs/Server.cpp \
				srcs/Client.cpp \
				srcs/Channel.cpp \
				srcs/join.cpp \
				srcs/message.cpp \
				srcs/sender.cpp \
				srcs/utils.cpp \
				srcs/pong.cpp \
				srcs/prvmsg.cpp \
				srcs/nick.cpp \
				srcs/mode.cpp \
				srcs/invite.cpp \
				srcs/topic.cpp \
				srcs/notice.cpp \
				srcs/part.cpp \
				srcs/oper.cpp \
				srcs/quit.cpp \
				srcs/kill.cpp

CC			= c++
CFLAGS		= #-Wall -Wextra -Werror -std=c++98

all: $(NAME)
$(NAME): $(SRCSDIR)/$(SRCS)
	@echo "Assembling $@"
	@$(CC) $(CFLAGS) -o $@ $^
clean:
fclean: clean
	rm -rf $(NAME)
re: fclean all
.PHONY: all clean fclean re test