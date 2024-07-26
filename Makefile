CC = clang
CFLAGS = -Wall -Wextra -Werror -g3
SRC = ft_malcolm.c
NAME = ft_malcolm

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re