NAME = prog
CC = g++
FLAGS = -std=c++11 #-W -Wall -Wextra -Werror
SFML = -I/usr/local/Cellar/sfml/2.5.1_1/include -L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system
RM = rm -f
SRCS =	main.cpp \
# SRCS =	Color.cpp \
# 		Point.cpp \
# 		Plan.cpp \
# 		main.cpp

OBJS = ${SRCS:.cpp=.o}

.cpp.o:
	${CC} ${FLAGS} ${SFML} -c $< -o ${<:.cpp=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${FLAGS} ${SFML} -o ${NAME} ${OBJS}

clean:
	${RM} *.o

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY	: all clean fclean re
