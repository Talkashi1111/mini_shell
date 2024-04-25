/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:56:57 by achappui          #+#    #+#             */
/*   Updated: 2024/04/25 21:02:23 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_subshell_error(char *str, t_minishell *info)
{
	info->last_exit_status = errno;
	ft_fprintf(info->saved_streams[1], str, strerror(errno));
	return (info->last_exit_status);
}

int	handle_subshell(t_node *node, t_minishell *info)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
		return (handle_subshell_error("fork: %s\n", info));
	if (pid == 0)
	{
		info->last_exit_status = ft_run(node->child[0], info);
		ft_exit(NULL, info);
	}
	if (waitpid(pid, &status, 0) == -1)
		return (handle_subshell_error("waitpid: %s\n", info));
	if (WIFEXITED(status))
		info->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		info->last_exit_status = 128 + WTERMSIG(status);
	else
		ft_fprintf(info->saved_streams[1], "Child process exited unknown\n");
	return (info->last_exit_status);
}
