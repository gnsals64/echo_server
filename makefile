SERVER_NAME = server
CLIENT_NAME = client
CC = c++
FLAG = -Wall -Werror -Wextra

SERVER_FILES = $(addprefix ./, server)
CLIENT_FILES = $(addprefix ./, client)

SERVER_SRCS = ${SERVER_FILES:=.cpp}
CLIENT_SRCS = ${CLIENT_FILES:=.cpp}

all :
	@echo "Usage : make [Option]\n"
	@echo "Option:"
	@echo "     server  make chat server"
	@echo "     client  make chat client"

server :
	${CC} ${SERVER_SRCS} ${INC} -o ${SERVER_NAME}

client :
	${CC} ${CLIENT_SRCS} ${INC} -o ${CLIENT_NAME}

fclean :
	rm -f ${SERVER_NAME} ${CLIENT_NAME}

re : fclean server client

.PHONY : all clean fclean re
