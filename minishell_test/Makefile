NAME	=	minishell

SRC		=	minishell.c
OBJ		=	$(addprefix object/, $(SRC:%.c=%.o))

CFLAGS	=	-Wall -Wextra -Werror -I libft
CC		=	gcc

all: $(NAME)

object/%.o: %.c | object
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) -L libft -lft -lreadline $^ -o $@

object:
	mkdir -p object

clean:
	make clean -C libft
	rm -rf object

fclean: clean
	make fclean -C libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
