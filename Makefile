CC = clang
CFLAGS = -Wall -Wextra -Werror -g3
SRC = ft_malcolm.c ft_utils.c
OBJ = $(SRC:.c=.o)
LIBFT = ./libft/libft.a
NAME = ft_malcolm

NONE='\033[0m'
GREEN='\033[32m'
YELLOW='\033[33m'
GRAY='\033[2;37m'
CURSIVE='\033[3m'

all: $(NAME)

$(OBJ): %.o: %.c
	@ echo $(CURSIVE) $(GRAY) " - Making object files..." $(NONE)
	@ $(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJ) $(LIBFT)
	@ echo $(CURSIVE) $(GRAY) " - Compiling $(NAME)..." $(NONE)
	@ $(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@ echo $(GREEN)" - Compiled - " $(NONE)

$(LIBFT):
	@ echo $(CURSIVE) $(GRAY) " - Compiling $(LIBFT)..." $(NONE)
	@ make all -C ./libft

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	make fclean -C ./libft

re: fclean all

.PHONY: all clean fclean re