/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 23:24:44 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/07 17:41:02 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "minishell.h"

void	ft_free_pipes(t_node *node, t_minishell *info)
{
	unsigned int	i;

	if (!info->fd_pipe)
		return ;
	i = 0;
	while (i < node->pipe_nb)
	{
		if (info->fd_pipe[i][PIPE_OUT] >= 0 &&
				close(info->fd_pipe[i][PIPE_OUT]) < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
				info->fd_pipe[i][PIPE_OUT], "pipe", strerror(errno));
		}
		if (info->fd_pipe[i][PIPE_IN] >= 0 &&
				close(info->fd_pipe[i][PIPE_IN]) < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
				info->fd_pipe[i][PIPE_IN], "pipe", strerror(errno));
		}
		i++;
	}
	free(info->fd_pipe);
	info->fd_pipe = NULL;
	info->pipe_nb = 0;
}

int	ft_open_pipes(t_node *node, t_minishell *info)
{
	unsigned int	i;

	info->fd_pipe = malloc((node->pipe_nb) * sizeof(int [2]));
	if (!info->fd_pipe)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "failed alloc pipes: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	i = 0;
	while (i < node->pipe_nb)
	{
		info->fd_pipe[i][PIPE_OUT] = -1;
		info->fd_pipe[i++][PIPE_IN] = -1;
	}
	i = 0;
	while (i < node->pipe_nb)
	{
		if (pipe(info->fd_pipe[i]) < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "pipe %d: %s\n", i, strerror(errno));
			return (info->last_exit_status);
		}
		i++;
	}
	info->pipe_nb = node->pipe_nb;
	info->last_exit_status = OK;
	return (OK);
}