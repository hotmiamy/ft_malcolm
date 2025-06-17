SRC 		=	$(addprefix ./lib/, $(SRCFILES))
OBJFILES 	=	$(SRC:.c=.o)
SRCFILES	=	ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
				ft_isprint.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strlcat.c \
				ft_strlcpy.c ft_strlen.c ft_toupper.c ft_tolower.c ft_strchr.c \
				ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
				ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
				ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
				ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
				ft_itoa_base_converter.c ft_uitoa.c ft_gnl.c ft_isxdigit.c

BONUS			 = $(addprefix ./lib/, $(BONUS_SRC))
BONUS_SRC		 = ft_lstnew.c ft_lstdelone.c ft_lstclear.c ft_lstadd_front.c \
					ft_lstadd_back.c ft_lstsize.c ft_lstiter.c ft_lstmap.c ft_lstlast.c
BONUS_OBJ		 = $(BONUS:.c=.o)

SRC_PRINT 	 =	$(addprefix ./ft_printf/, $(SRC_PRINT_F))
SRC_PRINT_F  =	ft_flags.c ft_printf.c ft_types.c
PRINT_OBJS	 =	$(SRC_PRINT:.c=.o)
PRINT_HEADER =	ft_printf.h

CFLAGS		=	-Wall -Wextra -Werror -g
CC			=	clang
NAME		=	libft.a

AR			=	ar
RM			=	rm -rf
ARFLAGS		=	rcs

NONE		=	'\033[0m'
RED 		=	'\033[1;31m'
GREEN		=	'\033[32m'
YELLOW		=	'\033[33m'
GRAY		=	'\033[2;37m'
CURSIVE		=	'\033[3m'

all: $(NAME)

%.o: %.c
	@ echo $(CURSIVE) $(YELLOW) " - Making object $<..." $(NONE)
	@ $(CC) $(CFLAGS) -c $< -o $@
	@ echo $(CURSIVE) $(GREEN) " - Objects Created" $(NONE)

$(NAME): $(OBJFILES) $(PRINT_OBJS) Makefile
	@ echo $(CURSIVE) $(GRAY) " - Compressing Objects in $(NAME)..." $(NONE)
	@ $(AR) $(ARFLAGS) $(NAME) $(OBJFILES) $(PRINT_OBJS)
	@ echo $(GREEN)" - Compressed - " $(NONE)

re: fclean all

bonus: $(BONUS_OBJ)
	@ echo $(CURSIVE) $(YELLOW) " - Compressing Bonus Objects in $(NAME)..." $(NONE)
	@ $(AR) $(ARFLAGS) $(NAME) $(BONUS_OBJ)
	@ echo $(GREEN)" - Compressed - " $(NONE)

clean:
	@ echo $(CURSIVE) $(RED) " - Removing Object files of libft..." $(NONE)
	@ $(RM) $(OBJFILES)
	@ echo $(CURSIVE) $(RED) " - Removing Object files of ft_printf..." $(NONE)
	@ $(RM) $(PRINT_OBJS)
	@ echo $(CURSIVE) $(RED) " - Removing Object files of libft bonus..." $(NONE)
	@ $(RM) $(BONUS_OBJ)
	@ echo $(GREEN) " - Objects deleted !" $(NONE)

fclean: clean
	@ echo $(CURSIVE) $(RED) " - removing $(NAME)..." $(NONE)
	@ $(RM) $(NAME)
	@ echo $(GREEN) " - $(NAME) deleted !" $(NONE)

so:
	$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRCFILES)
	gcc -nostartfiles -shared -o libft.so $(OBJFILES)

.PHONY: all re clean fclean
