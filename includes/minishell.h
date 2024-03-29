/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:37:04 by achappui          #+#    #+#             */
/*   Updated: 2024/03/29 14:33:46 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <limits.h>

# ifdef PATH_MAX
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
};
int cd_builtin(char *args[], char *envp[]);

#endif