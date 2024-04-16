/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:59:33 by achappui          #+#    #+#             */
/*   Updated: 2024/04/16 20:09:02 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int ft_wait_pid(int child_pid, t_minishell *info)
{
	int pid_tmp;
	int status;

	while (TRUE)
	{
		pid_tmp = waitpid(-1, &status, 0);
		if (pid_tmp == -1) {
			if (errno == ECHILD)
				break ;
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
			break ;
		}
		if (pid_tmp != child_pid)
			continue ;
		if (WIFEXITED(status))
			info->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			info->last_exit_status = 128 + WTERMSIG(status);
		else
			ft_fprintf(STDERR_FILENO, "Child process exited with unknown status\n");
	}
	return (info->last_exit_status);
}

unsigned int	token_list_size(t_token_list *list)
{
	unsigned int	size;

	size = 0;
	while (list)
	{
		size++;
		list = list->next;
	}
	return (size);
}

char	**token_list_to_args(t_token_list *node, t_minishell *info)
{
	unsigned int	size;
	unsigned int	i;
	char			**args;

	size = token_list_size(node);
	args = ft_calloc((size + 1), sizeof(char *));
	if (!args)
	{
		info->last_exit_status = MALLOC_ERROR;
		return (NULL);
	}
	i = 0;
	while (node)
	{
		args[i] = ft_strdup(node->str);
		if (!args[i])
		{
			info->last_exit_status = MALLOC_ERROR;
			free_args(args);
			return (NULL);
		}
		node = node->next;
		i++;
	}
	return (args);
}

void	reset_fd_tabs(int heredoc_pipe[2], int saved_std[2])
{
	heredoc_pipe[0] = -1;
	heredoc_pipe[1] = -1;
	saved_std[0] = -1;
	saved_std[1] = -1;
}

void    ft_close_size_2(int fds[2], t_minishell *info)
{
    if (fds[0] >= 0 && close(fds[0]) < 0)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
            fds[0], info->token_list->str, strerror(errno));
    }
    fds[0]= -1;
    if (fds[1] >= 0 && close(fds[1]) < 0)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
            fds[1], info->token_list->str, strerror(errno));
    }
    fds[1] = -1;
}