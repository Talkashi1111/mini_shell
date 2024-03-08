NAME      	=	libft.a

SRC_LIBFT	=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
				ft_isprint.c ft_toupper.c ft_tolower.c ft_strlen.c \
				ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
				ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c \
				ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
				ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
				ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
				ft_putendl_fd.c ft_putnbr_fd.c \
				ft_lstnew.c ft_lstadd_front.c ft_lstsize.c \
				ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
				ft_lstclear.c ft_lstiter.c ft_lstmap.c

SRC_PRINTF	= 	ft_printf/ft_printf.c ft_printf/ft_options.c \
				ft_printf/ft_print_integers.c ft_printf/ft_print_letters.c

SRC_GNL		=	get_next_line/get_next_line.c get_next_line/get_next_line_utils.c

OBJ			=	$(addprefix libft/, $(notdir $(SRC_LIBFT:%.c=%.o))) \
				$(addprefix ft_printf/, $(notdir $(SRC_PRINTF:%.c=%.o))) \
				$(addprefix get_next_line/, $(notdir $(SRC_GNL:%.c=%.o)))

DEP			=	$(OBJ:.o=.d)

CFLAGS		=	-Wall -Wextra -Werror
CC			=	gcc

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $*.d

-include $(DEP)

$(NAME): $(OBJ)
	ar rcs $@ $^

clean:
	rm -f $(OBJ) $(DEP)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
