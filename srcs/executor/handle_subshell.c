/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:56:57 by achappui          #+#    #+#             */
/*   Updated: 2024/04/17 22:41:16 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_subshell(t_node *node, t_minishell *info)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "fork: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	if (pid == 0)
	{
		info->last_exit_status = ft_run(node->child[0], info);
		ft_exit(NULL, info);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	if (WIFEXITED(status))
		info->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		info->last_exit_status = 128 + WTERMSIG(status);
	else
		ft_fprintf(STDERR_FILENO, "Child process exited with unknown status\n");
	return (info->last_exit_status);
}