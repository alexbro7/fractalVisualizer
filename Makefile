# NAME = fractalVisualizer
# CC = g++
# FLAGS = -std=c++11 -g3 -fsanitize=address #-W -Wall -Wextra -Werror
# SFML = -I/usr/local/Cellar/sfml/2.5.1_1/include -L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system
# RM = rm -f
# SRCS =	srcs/main.cpp \
# # SRCS =	Color.cpp \
# # 		Point.cpp \
# # 		Plan.cpp \
# # 		main.cpp

# OBJS = ${SRCS:.cpp=.o}

# .cpp.o:
# 	${CC} ${FLAGS} ${SFML} -c $< -o ${<:.cpp=.o}

# all: ${NAME}

# ${NAME}: ${OBJS}
# 	${CC} ${FLAGS} ${SFML} -o ${NAME} ${OBJS}

# clean:
# 	${RM} *.o

# fclean: clean
# 	${RM} ${NAME}

# re: fclean all

# .PHONY	: all clean fclean re
#  |  |  ___ \    \  |         |
#  |  |     ) |  |\/ |   _  |  |  /   _ 
# ___ __|  __/   |   |  (   |    <    __/ 
#    _|  _____| _|  _| \__,_| _|\_\ \___|
#                              by jcluzet
################################################################################
#                                     CONFIG                                   #
################################################################################

NAME        := fractol
CC         := c++
MACOS_FLAGS    := -std=c++11 -I/usr/local/Cellar/sfml/2.5.1_1/include/ -L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system
MACARM_FLAGS    := -std=c++11 -I/opt/homebrew/Cellar/sfml/2.5.1_1/include/ -L/opt/homebrew/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system
LINUX_FLAGS    :=

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

SRCS        :=      srcs/main.cpp \
                          
OBJS        := ${SRCS:.cpp=.o}


################################################################################
#                                  Makefile  objs                              #
################################################################################


CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
RM		    := rm -f
UNAME = $(shell uname)
ARM = $(shell uname -m)


ifeq ($(UNAME), Darwin)
	ifeq ($(ARM), x86_64)
$(NAME):
			@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			${CC} ${MACARM_FLAGS} -o ${NAME} ${SRCS}
			@echo "$(GREEN)$(NAME) created[0m ✔️"
	else
$(NAME):
			@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			${CC} ${MACARM_FLAGS} -o ${NAME} ${SRCS}
			@echo "$(GREEN)$(NAME) created[0m ✔️"
endif
endif

ifeq ($(UNAME), Linux)))
$(NAME):
			@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			g++ -c srcs/main.cpp -I include/ 
g++ main.o -o sfml-app -L lib/ -lsfml-graphics -lsfml-window -lsfml-system
export LD_LIBRARY_PATH=lib/ && ./sfml-app
			@echo "$(GREEN)$(NAME) created[0m ✔️"
endif

all:		${NAME}

bonus:		all

clean:
			@ ${RM} *.o */*.o */*/*.o
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)objs ✔️"

fclean:		clean
			@ ${RM} ${NAME}
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)binary ✔️"

re:			fclean all

.PHONY:		all clean fclean re


