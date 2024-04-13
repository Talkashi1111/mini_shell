/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:10:11 by achappui          #+#    #+#             */
/*   Updated: 2024/04/13 19:26:58 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <errno.h>

int	heredoc_exists(t_token_list *redi)
{
	while (redi)
	{
		if (redi->type == STDIN_HEREDOC)
			return (1);
		redi = redi->next;
	}
	return (0);
}



int	heredoc_fonc()
{
	
}

int	run_heredocs(t_node *node, int heredoc_pipe[2], t_minishell *info)
{
	pid_t	pid;

	if (!heredoc_exists(node->redi))
		return (OK);
	if (pipe(heredoc_pipe) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
		return (info->last_exit_status);	
	}
	pid = fork();
	if (pid == -1)
	{
		
	}
	else if (pid == 0)
	{
		heredoc_fonc();
		ft_exit();
	}
	wait
	return (OK);
}
























int	redirection_heredoc(t_node *node, t_minishell *info, int heredoc_pipe[2])
{
	t_token_list	*index;
	char			*line;
	unsigned int	i;

	i = number_of_herdocs();
	index = get_next_heredoc();
	while (index)
	{
		while ()
		{
			if (i == 1)
			{
				if (write(heredoc_pipe[1], line, ft_strlen(line)) == -1)
				{
					info->last_exit_status = errno;
					ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
					return (info->last_exit_status);	
				}
			}
			else
				free(line);
		}
		index = get_next_heredoc();	
	}
}