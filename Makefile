NAME        := fractalVisualizer
CC         := c++
MACOS_FLAGS    := -std=c++11 -I/usr/local/Cellar/sfml/2.5.1_1/include/ -L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system # -Wall -Werror -Wextra -W
MACARM_FLAGS   := -std=c++11 -I/opt/homebrew/Cellar/sfml/2.5.1_1/include/ -L/opt/homebrew/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system # -Wall -Werror -Wextra -W
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


$(NAME):
			@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			@${CC} ${MACARM_FLAGS} -o ${NAME} ${SRCS}
			@echo "$(GREEN)$(NAME) created[0m ✔️"

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


