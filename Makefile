NAME = ircserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 -pedantic -fsanitize=address -g3

SRC = 	main.cpp \

OBJ = $(SRC:.cpp=.o)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re
