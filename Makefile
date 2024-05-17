NAME := minishell
BONUS_NAME := minishell_bonus
OBJECT_DIR := obj
LIBFT_DIR := libft
INCLUDE_DIR := includes
SRC_DIR := srcs
BONUS_DIR := bonus
FILES := main.c \
		init_minishell.c \
		builtin/cd_builtin.c \
		builtin/echo_builtin.c \
		builtin/env_builtin.c \
		builtin/pwd_builtin.c \
		builtin/exit_builtin.c \
		builtin/export_builtin.c \
		builtin/unset_builtin.c \
		builtin/builtin_utils.c \
		builtin/utils.c \
		lexer/syntax_analyser.c \
		lexer/tokenizer_1.c \
		lexer/tokenizer_2.c \
		lexer/tokenizer_3.c \
		lexer/tokenizer_utils.c \
		parser/make_tree_1.c \
		parser/make_tree_2.c \
		parser/make_tree_3.c \
		executor/expand_dollars.c \
		executor/expand_dollars_utils.c \
		executor/expand_quotes.c \
		executor/expand_wildcards.c \
		executor/expand_wildcards_utils.c \
		executor/handle_cmd_1.c \
		executor/handle_cmd_2.c \
		executor/handle_cmd_3.c \
		executor/handle_pipex.c \
		executor/handle_pipex_utils.c \
		executor/handle_subshell.c \
		executor/redirections.c \
		executor/run_tree.c \
		parser/heredoc.c \
		parser/heredoc_utils.c \
		debug.c \
		debug_2.c
SRCS := $(addprefix $(SRC_DIR)/, $(FILES))
CFLAGS := -Wall -Wextra -Werror -DHEREDOC_PATH='"$(shell pwd)/heredocs"'
ifdef DEBUG
    CFLAGS += -DDEBUG=1 -g
endif
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	LEAK_TOOL := leaks -atExit --
	HOSTNAME := $(shell whoami)
    READLINE_LIB := /Users/$(HOSTNAME)/.brew/opt/readline/lib
    READLINE_INC := /Users/$(HOSTNAME)/.brew/opt/readline/include
else
	LEAK_TOOL := valgrind --leak-check=full
	READLINE_LIB := /lib/x86_64-linux-gnu
    READLINE_INC := /usr/include/readline
endif
IFLAGS := -I$(LIBFT_DIR) -I$(INCLUDE_DIR) -I$(READLINE_INC)
LFLAGS := -L$(LIBFT_DIR) -lft -L$(READLINE_LIB) -lreadline
OBJECTS := $(addprefix $(OBJECT_DIR)/,$(FILES:.c=.o))
OBJS_WITHOUT_MAIN := $(filter-out $(OBJECT_DIR)/main.o, $(OBJECTS))
BONUS_OBJ :=  $(addprefix $(BONUS_DIR)/,$(BONUS_FILES:.c=.o))
CC := gcc
#color codes
END := \033[0m
WHITE := \033[1;37m
NC := \033[0m
PINK := \033[1;35m
GREEN := \033[32m
BOLD := \033[1m
L_PURPLE :=\033[38;5;55m


$(info $(OBJECTS))

.PHONY: all
all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o $(NAME)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	@echo -e "${L_PURPLE}${BOLD}Compiling:${NC} $< -> $@"
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@  && \
		echo -e "${GREEN}Success:${NC} Compiled $<" || \
		(echo -e "${PINK}Error:${NC} Failed to compile $<"; exit 1)

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
