# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/30 14:51:12 by tkashi            #+#    #+#              #
#    Updated: 2023/10/31 14:26:40 by tkashi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME := libft.a
SRCS := ft_isalnum.c \
	ft_isalpha.c \
	ft_isascii.c \
	ft_isdigit.c \
	ft_isprint.c \
	ft_tolower.c \
	ft_toupper.c \
	ft_bzero.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_memset.c \
	ft_strlcat.c \
	ft_strlcpy.c \
	ft_strlen.c \
	ft_strncmp.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_calloc.c \
	ft_atoi.c \
	ft_strnstr.c \
	ft_strdup.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_strtrim.c \
	ft_split.c \
	ft_strmapi.c \
	ft_striteri.c \
	ft_putchar_fd.c \
	ft_putstr_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_itoa.c \
	ft_printf.c \
	ft_print_d.c \
	ft_print_x.c \
	ft_print_s.c \
	ft_print_u.c \
	ft_print_c.c \
	ft_print_p.c \
	get_next_line.c \
	get_next_line_utils.c
	
BONUS_SRCS := ft_lstnew_bonus.c \
	ft_lstadd_front_bonus.c \
	ft_lstsize_bonus.c \
	ft_lstlast_bonus.c \
	ft_lstadd_back_bonus.c \
	ft_lstdelone_bonus.c \
	ft_lstclear_bonus.c \
	ft_lstiter_bonus.c \
	ft_lstmap_bonus.c

CC := cc
CFLAGS := -Wall -Wextra -Werror 
OBJECTS := $(SRCS:.c=.o)
BONUS_OBJS := $(BONUS_SRCS:.c=.o)

.PHONY: all
all: $(NAME)

$(NAME): $(OBJECTS)
	ar -rcs $@ $^

.PHONY: bonus
bonus: $(OBJECTS) $(BONUS_OBJS)
	ar -rcs $(NAME) $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: re
re: fclean all

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(BONUS_OBJS)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)
	rm -f ft_test
	rm -f ft_test_bonus

ft_test: $(NAME) main.c
	$(CC) -g main.c -L. -lft -o ft_test

ft_test_bonus: bonus main_bonus.c
	$(CC) -g main_bonus.c -L. -lft -o ft_test_bonus 

.PHONY: test
test: ft_test
	./ft_test

.PHONY: test_bonus
test_bonus: ft_test_bonus
	./ft_test_bonus

.PHONY: leak
leak: ft_test
	valgrind --leak-check=full ./ft_test
