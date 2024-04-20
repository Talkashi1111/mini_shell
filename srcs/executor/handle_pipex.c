/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:55:31 by achappui          #+#    #+#             */
/*   Updated: 2024/04/20 11:07:52 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_pipes(t_minishell *info)
{
	unsigned int	i;

	if (!info->fd_pipe)
		return ;
	i = 0;
	while (i < info->pipe_nb)
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

void	ft_child(t_node *node, unsigned int i, t_minishell *info)
{
	if (i > 0)
	{
		if (dup2(info->fd_pipe[i - 1][PIPE_OUT], STDIN_FILENO) < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
				info->fd_pipe[i - 1][PIPE_OUT], STDIN_FILENO, strerror(errno));
			ft_free_pipes(info);
			ft_exit(NULL, info);
		}
	}
	if (i < info->pipe_nb)
	{
		if (dup2(info->fd_pipe[i][PIPE_IN], STDOUT_FILENO) < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
				info->fd_pipe[i][PIPE_IN], STDOUT_FILENO, strerror(errno));
			ft_free_pipes(info);
			ft_exit(NULL, info);
		}
	}
	ft_free_pipes(info);
	info->last_exit_status = ft_run(node, info);
	ft_exit(NULL, info);
}

int	handle_pipex(t_node *node, t_minishell *info)
{
	unsigned int		i;
	int					pid;

	if (ft_open_pipes(node, info) != OK)
	{
		ft_free_pipes(info);
		return (info->last_exit_status);
	}
	i = 0;
	while (i < node->child_nb)
	{
		pid = fork();
		if (pid == 0)
			ft_child(node->child[i], i, info);
		else if (pid < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "fork: %s\n", strerror(errno));
			break ;
		}
		i++;
	}
	ft_free_pipes(info);
	return (ft_wait_pid(pid, info));
}