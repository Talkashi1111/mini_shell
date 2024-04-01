/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:37:04 by achappui          #+#    #+#             */
/*   Updated: 2024/04/01 22:05:38 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include "libft.h"

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define FALSE		0
# define TRUE		1

# define COLOR_GREEN "\033[0;32m"
# define COLOR_RESET "\033[0m"
# define COLOR_RED "\033[0;31m"

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
	unsigned int	pipe_nb; //as-ton vraiment besoin du nombre de pipes ici dans un node
	int				(*pipes)[2]; //as-ton vraiment besoin de malloc les pipes dans un node
	t_token_list	*args;
	t_token_list	*redi;
	struct s_node	**child;
}	t_node;

/* builtins */
int		count_args(char **args);
void    free_args(char **args);
int		ft_cd(char *args[], char **envp[]);
int 	ft_pwd(char *args[], char **envp[]);
int		ft_echo(char *args[], char **envp[]);
int		ft_env(char *args[], char **envp[]);
int		ft_export(char *args[], char **envp[]);
int 	ft_unset(char *args[], char **envp[]);
char	**copy_env(char *envp[]);
char	*find_envp_arg(char *envp[], char *str);
int		update_or_add_envp(char **envp[], char *str, char *new_val);
int		ft_getcwd(char *path, size_t size);

/* lexer */
t_token_list	*tokenizer(char *str);
char			get_token_type(char *str);
void			to_operator_end(char **end);
void			to_word_end(char **end);
void			free_token_list(t_token_list *begin);
void			skip_whitespace_start(char **start);
void			display_token_list(t_token_list *token);
char			syntax_analyser(t_token_list *token);
int				ft_isspace(char c);
#endif