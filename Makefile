NAME := minishell
BONUS_NAME := minishell_bonus
OBJECT_DIR := obj
LIBFT_DIR := libft
INCLUDE_DIR := includes
SRC_DIR := srcs
BONUS_DIR := bonus
FILES := main.c \
		builtin/cd_builtin.c \
		builtin/echo_builtin.c \
		builtin/env_builtin.c \
		builtin/pwd_builtin.c \
		builtin/export_builtin.c \
		builtin/unset_builtin.c \
		builtin/builtin_utils.c \
		builtin/utils.c \
		lexer/tokenizer.c \
		lexer/tokenizer_utils.c \
		lexer/syntax_analyser.c
SRCS := $(addprefix $(SRC_DIR)/, $(FILES))
BONUS_FILES :=
CFLAGS := -Wall -Wextra -Werror -g
IFLAGS := -I$(LIBFT_DIR) -I$(INCLUDE_DIR)
LFLAGS := -L$(LIBFT_DIR) -lft -lreadline
OBJECTS := $(addprefix $(OBJECT_DIR)/,$(FILES:.c=.o))
OBJS_WITHOUT_MAIN := $(filter-out $(OBJECT_DIR)/main.o, $(OBJECTS))
BONUS_OBJ :=  $(addprefix $(BONUS_DIR)/,$(BONUS_FILES:.c=.o))
CC := gcc
END := "\033[0m"
WHITE := "\033[1;37m"
NC := \033[0m
PINK := "\033[1;35m"
GREEN := "\033[32m"
BOLD := "\033[1m"
L_PURPLE :="\033[38;5;55m"
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	LEAK_TOOL := leaks -atExit --
else
	LEAK_TOOL := valgrind --leak-check=full
endif
$(info $(OBJECTS))


.PHONY: all 
all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o $(NAME)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@ 

$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@


.PHONY: bonus
bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ) $(OBJECTS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(OBJS_WITHOUT_MAIN) $(LFLAGS) -o $(BONUS_NAME) 

.PHONY: re
re: fclean all

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)
	rm -f $(BONUS_OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(BONUS_NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

.PHONY: leak
leak: $(NAME)
	$(LEAK_TOOL) ./$(NAME)