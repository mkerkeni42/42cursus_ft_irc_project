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
UP = \033[A

B1 = \033[0;36m
B2 = \033[0;34m
B3 = \033[0;94m
B4 = \033[1;34m
B5 = \033[38;5;39m

BACKGREEN = \033[45m

#------------------------------------SRCS-------------------------------------#

SRC = 	main.cpp \
		Network/NetworkServ.cpp \
		Messages/MessageServ.cpp \
		Messages/Replies.cpp \
		Messages/Welcome.cpp \
		Messages/Join.cpp \
		Messages/Part.cpp \
		Messages/Topic.cpp \
		Messages/Kick.cpp \
		Messages/Invite.cpp \
		Messages/Mode.cpp \
		Messages/Privmsg.cpp \
		Messages/Tools.cpp \
		User/User.cpp \
		User/UserServ.cpp \
		Channel/Channel.cpp \
		Channel/ChannelServ.cpp \

OBJ = $(SRC:.cpp=.o)

#----------------------------------COMPILER-----------------------------------#

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -pedantic -fsanitize=address -g3

.cpp.o:
	@printf "    $(BOLD)$(PINK)$(UNDERLINE)Compiling.$(DEF)  $(PURPLE)$(BOLD)ircserv$(DEF) ⏳  \r"
	@sleep 0.10
	@printf "    $(BOLD)$(PINK)$(UNDERLINE)Compiling..$(DEF)  $(PURPLE)$(BOLD)ircserv$(DEF) ⏳\r"
	@sleep 0.10
	@printf "    $(BOLD)$(PINK)$(UNDERLINE)Compiling...$(DEF)  $(PURPLE)$(BOLD)ircserv$(DEF) ⌛\r"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

#------------------------------------RULES------------------------------------#

NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	@echo ""
	@$(CXX) $(CXXFLAGS) -o $(NAME) $^
	@echo ""
	@echo "    $(YELLOW)$(BOLD)Compiled ! ✨$(DEF)"
	@echo "\n$(B2)         _        _                         _          _            _      $(DEF)"
	@echo "$(B2)        /\ \     /\ \                      /\ \       /\ \        /\ \     $(DEF)"
	@echo "$(B2)       /  \ \    \_\ \                     \ \ \     /  \ \      /  \ \    $(DEF)"
	@echo "$(B2)      / /\ \ \   /\__ \                    /\ \_\   / /\ \ \    / /\ \ \   $(DEF)"
	@echo "$(B2)     / / /\ \_\ / /_ \ \                  / /\/_/  / / /\ \_\  / / /\ \ \  $(DEF)"
	@echo "$(B2)    / /_/_ \/_// / /\ \ \                / / /    / / /_/ / / / / /  \ \_\ $(DEF)"
	@echo "$(B2)   / /____/\  / / /  \/_/               / / /    / / /__\/ / / / /    \/_/ $(DEF)"
	@echo "$(B2)  / /\____\/ / / /   ___________       / / /    / / /_____/ / / /          $(DEF)"
	@echo "$(B2) / / /      / / /___/__________/\  ___/ / /__  / / /\ \ \  / / /________   $(DEF)"
	@echo "$(B2)/ / /      /_/ //__________    \ \/\__\/_/___\/ / /  \ \ \/ / /_________\  $(DEF)"
	@echo "$(B2)\/_/       \_\/ \____\/    \____\/\/_________/\/_/    \_\/\/____________/  $(DEF)"

clean:
	@rm -f $(OBJ)
	@echo "	❌ $(BOLD)$(RED)Files .o moved to trash$(DEF) 🗑️"

fclean: clean
	@rm -f $(NAME)
	@echo "	❌ $(BOLD)$(RED)Deleted file .a$(DEF) 🗑️"

re: fclean all

.PHONY: all, clean, fclean, re
