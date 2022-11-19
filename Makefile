NAME		= ircserv
INC			= inc/Server.hpp \
				inc/Client.hpp \
				inc/Channel.hpp
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
				srcs/kill.cpp \
				srcs/kick.cpp \
				srcs/names.cpp \
				srcs/user.cpp \
				srcs/pass.cpp \
				srcs/list.cpp
OBJS		= ${SRCS:.cpp=.o}

CC			= c++
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
RM 			= rm -f
CFLAGS		= -Wall -Wextra -Werror -std=c++98

all: ${NAME}

$(NAME)	: ${OBJS} ${INC}
	${CC} ${CFLAGS} ${OBJS} -o $@

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
