/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:37:04 by achappui          #+#    #+#             */
/*   Updated: 2024/03/31 19:19:37 by tkashi           ###   ########.fr       */
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
};
int		count_args(char **args);
void    free_args(char **args);
int		ft_cd(char *args[], char **envp[]);
int 	ft_pwd(char *args[], char **envp[]);
int		ft_echo(char *args[], char **envp[]);
int		ft_env(char *args[], char **envp[]);
int		ft_export(char *args[], char **envp[]);
int ft_unset(char *args[], char **envp[]);
char	**copy_env(char *envp[]);
char	*find_envp_arg(char *envp[], char *str);
int		update_or_add_envp(char **envp[], char *str, char *new_val);
int		ft_getcwd(char *path, size_t size);
#endif