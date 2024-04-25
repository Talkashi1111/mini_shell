/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipex_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 16:48:46 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 13:17:01 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe_in(t_minishell *info, unsigned int i)
{
	if (info->fd_pipe[i][PIPE_IN] >= 0 &&
		close(info->fd_pipe[i][PIPE_IN]) < 0)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "close(%d): %s: %s\n",
			info->fd_pipe[i][PIPE_IN], "pipe", strerror(errno));
	}
}

int	ft_open_pipes_util(t_minishell *info, t_node *node)
{
	unsigned int	i;

	i = 0;
	while (i < node->pipe_nb)
	{
		if (pipe(info->fd_pipe[i]) < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(info->saved_streams[1],
				"pipe %d: %s\n", i, strerror(errno));
			return (info->last_exit_status);
		}
		i++;
	}
	info->pipe_nb = node->pipe_nb;
	return (OK);
}

void	close_saved_streams(t_minishell *info, int fd)
{
	if (close(fd) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "close: %s\n", strerror(errno));
	}
}
