#------------------------------------COLORS------------------------------------#

DEF = \033[0m
BOLD = \033[1m
UNDERLINE = \033[4m
RED = \033[31;2m
BLACK = \033[38;5;238m
BLUE = \033[38;5;153m
GREEN = \033[38;5;48m
YELLOW = \033[38;5;226m
ORANGE = \033[38;5;202m
PINK = \033[38;5;198m
PURPLE = \033[38;5;147m
UP = UP = \033[A

#------------------------------------SRCS-------------------------------------#

SRC = 	main.cpp \
		Channel/Channel.cpp \
		Channel/ChannelServ.cpp \

OBJ = $(SRC:.cpp=.o)

#----------------------------------COMPILER-----------------------------------#

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -pedantic -fsanitize=address -g3

.cpp.o:
	@$(CXX) $(CXXFLAGS) -c $< -o $@

#------------------------------------RULES------------------------------------#

NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	@echo ""
	@echo "	$(BOLD)$(PINK)$(UNDERLINE)Compiling...$(DEF)  $(PURPLE)$(BOLD)ircserv$(DEF) ⏳"
	@$(CXX) $(CXXFLAGS) -o $(NAME) $^
	@echo ""
	@echo "	$(YELLOW)$(BOLD)Compiled ! ✨$(DEF)"

clean:
	@rm -f $(OBJ)
	@echo "	❌ $(BOLD)$(RED)Files .o moved to trash$(DEF) 🗑️"

fclean: clean
	@rm -f $(NAME)
	@echo "	❌ $(BOLD)$(RED)Deleted file .a$(DEF) 🗑️"

re: fclean all

.PHONY: all, clean, fclean, re
