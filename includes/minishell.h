/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:37:04 by achappui          #+#    #+#             */
/*   Updated: 2024/04/15 18:39:52 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include "libft.h"

# ifndef DEBUG
#  define DEBUG 0
# endif

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define FALSE		0
# define TRUE		1
# define PIPE_OUT	0
# define PIPE_IN	1

# define COLOR_GREEN "\033[0;32m"
# define COLOR_RESET "\033[0m"
# define COLOR_RED "\033[0;31m"

enum tokenizer{
	HEAD,
	TAIL,
	TOKEN,
};

enum {
	OK,
	USAGE_ERROR,
	INIT_ERROR,
	MALLOC_ERROR,
	NOT_FOUND,
	SYNTAX_ERROR,
};

enum token_type
{
	WORD,
	STDIN,
	STDIN_HEREDOC,
	STDOUT,
	STDOUT_APPEND,
	OR,
	AND,
	PIPE,
	OPENPAR,
	CLOSEPAR,
	CMD,
};

typedef struct s_token_list
{
	char				*str;
	char				type;
	struct s_token_list	*next;
}	t_token_list;

typedef struct s_node
{
	char			type;
	t_token_list	*redi;
	t_token_list	*args;
	unsigned int	pipe_nb;
	unsigned int	child_nb;
	struct s_node	**child;
}	t_node;

struct	s_minishell;

typedef int (*t_pfunc)(char *args[], struct s_minishell *info);

typedef struct 
{
	char *name;
	t_pfunc func;
}	t_builtin;

typedef struct s_minishell
{
	char				**envp;
    int					last_exit_status;
    struct s_token_list	*token_list;
    struct s_node		*tree;
	t_builtin			builtins[8];
	int					(*fd_pipe)[2];
	unsigned int		pipe_nb;
}	t_minishell;

/* builtins */
int		count_args(char **args);
void    free_args(char **args);
int		ft_exit(char *args[], t_minishell *info);
int		ft_cd(char *args[], t_minishell *info);
int 	ft_pwd(char *args[], t_minishell *info);
int		ft_echo(char *args[], t_minishell *info);
int		ft_env(char *args[], t_minishell *info);
int		ft_export(char *args[], t_minishell *info);
int		print_sorted_envp(char *envp[]);
void	ft_buble_sort(char **sorted_envp);
int 	ft_unset(char *args[], t_minishell *info);
char	**copy_env(char *envp[]);
char 	*find_envp_arg(char *envp[], char *var_name, unsigned int var_name_len);
int		update_or_add_envp(t_minishell *info, char *str, char *new_val);
int		ft_getcwd(char *path, size_t size);
t_pfunc	is_builtin(char *str, t_builtin *builtin);
const char *node_type_to_string(enum token_type type);

/* lexer */
void			add_back_tokenizer(t_token_list *tl[3]);
t_token_list	*tokenizer(char *str);
char			get_token_type(char *str);
void			to_operator_end(char **end);
void			to_word_end(char **end);
void			free_token_list(t_token_list *node);
t_token_list	*create_token(void);
t_token_list	*new_token(char *str, char type);
t_token_list	*copy_token(t_token_list *token);
void			skip_whitespace_start(char **start);
void			display_token_list(t_token_list *token);
unsigned int	token_list_size(t_token_list *list);
char			syntax_analyser(t_token_list *token);
int				ft_isspace(char c);
char			*to_end_of_quote(char *str);
char			**tokens_to_args(t_token_list *token_list);

/* parser */
t_node			*handle_cmd(t_token_list *start, t_token_list *end);
t_node			*handle_parenthesis(t_token_list *start);
t_node			*handle_operator(t_token_list *start, t_token_list *end, t_token_list *tmp_token);
t_node			*handle_pipe(t_token_list *start, t_token_list *end, unsigned int	pipe_nb);
t_node			*new_tree_node(char type, unsigned int child_nb, unsigned int pipe_nb);
void			add_back_token_list(t_token_list **list, t_token_list *token);
t_token_list	*skip_parenthesis(t_token_list *token);
int				pipe_children(t_token_list *start, t_token_list *end, t_node *pipe_node);
int				init_cmd_node(t_token_list *start, t_token_list *end, t_node *cmd_node);
t_node			*tree_maker(t_token_list *start, t_token_list *end);
void			display_tree(t_node *node);
void			free_tree(t_node *node);

/* executor */
char			**token_list_to_args(t_token_list *node, t_minishell *info);
int				apply_redirections(t_node *node, t_minishell *info, int heredoc_pipe[2]);
int				ft_run(t_node *node, t_minishell *info);
void			ft_free_pipes(t_minishell *info);
int				ft_open_pipes(t_node *node, t_minishell *info);
int				wildcard_handler(t_token_list **token, t_minishell *info);
int				remove_quotes(t_token_list *args, t_minishell *info);
char			expand_dollars(t_token_list **args, t_minishell *info);
int 			execute_non_builtin(t_node *node, t_minishell *info, int heredoc_pipe[2]);
void    		free_tokens_and_tree(t_minishell *info);
int 			ft_wait_pid(int child_pid, t_minishell *info);
#endif
