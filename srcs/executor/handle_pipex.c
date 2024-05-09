/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:55:31 by achappui          #+#    #+#             */
/*   Updated: 2024/05/09 21:08:54 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_pipe(int pipe[2], t_minishell *info)
{
	if (pipe[0] != -1 && close(pipe[0]) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
	}
	pipe[0] = -1;
	if (pipe[1] != -1 && close(pipe[1]) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
	}
	pipe[1] = -1;
}

void	ft_child(t_node *node, int p[4], t_minishell *info)
{
	if ((p[0] != -1 && dup2(p[0], STDIN_FILENO) == -1) || \
		(p[3] != -1 && dup2(p[3], STDOUT_FILENO) == -1))
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
		ft_close_pipe(p, info);
		ft_close_pipe(p + 2, info);
		ft_exit(NULL, info);
	}
	ft_close_pipe(p, info);
	ft_close_pipe(p + 2, info);
	info->last_exit_status = ft_run(node, info);
	ft_exit(NULL, info);
}

int	ft_prepare_pipes(t_node *node, unsigned int i, int pipes_tab[4], \
					t_minishell *info)
{
	ft_close_pipe(pipes_tab, info);
	pipes_tab[0] = pipes_tab[2];
	pipes_tab[1] = pipes_tab[3];
	pipes_tab[2] = -1;
	pipes_tab[3] = -1;
	if (i < node->child_nb - 1 && pipe(pipes_tab + 2) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "pipe: %s\n", strerror(errno));
		return (1);
	}
	return (OK);
}

void	init_pipes_tab(int pipes_tab[4])
{
	pipes_tab[0] = -1;
	pipes_tab[1] = -1;
	pipes_tab[2] = -1;
	pipes_tab[3] = -1;
}

int	handle_pipex(t_node *node, t_minishell *info)
{
	unsigned int	i;
	int				pid;
	int				pipes_tab[4];

	i = 0;
	init_pipes_tab(pipes_tab);
	while (i < node->child_nb)
	{
		if (ft_prepare_pipes(node, i, pipes_tab, info) != OK)
			break ;
		pid = fork();
		if (pid == 0)
			ft_child(node->child[i], pipes_tab, info);
		else if (pid < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "fork: %s\n", strerror(errno));
			break ;
		}
		i++;
	}
	ft_close_pipe(pipes_tab, info);
	ft_close_pipe(pipes_tab + 2, info);
	return (ft_wait_pid(pid, info));
}
