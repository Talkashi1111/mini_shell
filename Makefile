NAME := minishell
BONUS_NAME := minishell_bonus
OBJECT_DIR := obj
LIBFT_DIR := libft
INCLUDE_DIR := includes
SRC_DIR := srcs
BUILTIN_DIR := builtin
BONUS_DIR := bonus
FILES := main.c \
BUILTIN_FILES := $(wildcard $(BUILTIN_DIR)/*.c)
SRCS := $(addprefix $(SRC_DIR)/, $(FILES)) $(BUILTIN_FILES)
BONUS_FILES :=
CFLAGS := -Wall -Wextra -Werror -g
IFLAGS := -I$(LIBFT_DIR) -I$(INCLUDE_DIR)
LFLAGS := -L$(LIBFT_DIR) -lft -lreadline
OBJECTS := $(addprefix $(OBJECT_DIR)/,$(FILES:.c=.o)) $(addprefix $(OBJECT_DIR)/,$(BUILTIN_FILES:.c=.o))
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

.PHONY: all 
all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o $(NAME)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c $(BUILTIN_DIR)/%.c 
	mkdir -p $(OBJECT_DIR)
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