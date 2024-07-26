CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC = ft_malcolm.c
OBJ = $(SRC:.c=.o)
NAME = ft_malcolm

all: $(NAME)

$(NAME): $(OBJ)
    $(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
    rm -f $(OBJ)

fclean: clean
    rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re