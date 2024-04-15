/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:45:47 by achappui          #+#    #+#             */
/*   Updated: 2024/04/13 22:07:25 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

const char	*node_type_to_string_expand(enum token_type type)
{
	if (type == AND)
		return ("AND");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == OPENPAR)
		return ("OPENPAR");
	else if (type == CLOSEPAR)
		return ("CLOSEPAR");
	else if (type == CMD)
		return ("CMD");
	else
		return ("UNKNOWN");
}

const char	*node_type_to_string(enum token_type type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == STDIN)
		return ("STDIN");
	else if (type == STDIN_HEREDOC)
		return ("STDIN_HEREDOC");
	else if (type == STDOUT)
		return ("STDOUT");
	else if (type == STDOUT_APPEND)
		return ("STDOUT_APPEND");
	else if (type == OR)
		return ("OR");
	else
		return (node_type_to_string_expand(type));
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\v'
		|| c == '\r' || c == '\f' || c == '\n')
		return (TRUE);
	return (FALSE);
}

int	count_args(char **args)
{
	int	i;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}
