/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:59:33 by achappui          #+#    #+#             */
/*   Updated: 2024/05/22 11:02:36 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_wait_pid(int child_pid, t_minishell *info)
{
	int	pid_tmp;
	int	status;

	while (TRUE)
	{
		pid_tmp = waitpid(-1, &status, 0);
		if (pid_tmp == -1)
		{
			if (errno == ECHILD)
				break ;
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO,
				"waitpid: %s\n", strerror(errno));
			break ;
		}
		if (pid_tmp != child_pid)
			continue ;
		if (WIFEXITED(status))
			info->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			info->last_exit_status = 128 + WTERMSIG(status);
		else
			ft_fprintf(STDERR_FILENO, "Child process exited\n");
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

char	**token_list_to_args(t_token_list *token_list, t_minishell *info)
{
	unsigned int	size;
	unsigned int	i;
	char			**args;

	size = token_list_size(token_list);
	args = ft_calloc((size + 1), sizeof(char *));
	if (!args)
	{
		info->last_exit_status = MALLOC_ERROR;
		return (NULL);
	}
	i = 0;
	while (token_list)
	{
		args[i] = ft_strdup(token_list->str);
		if (!args[i])
		{
			info->last_exit_status = MALLOC_ERROR;
			free_args(args);
			return (NULL);
		}
		token_list = token_list->next;
		i++;
	}
	return (args);
}

void	ft_close_fds(int fds[2], t_minishell *info)
{
	if (fds[0] != -1 && close(fds[0]) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			fds[0], info->token_list->str, strerror(errno));
	}
	fds[0] = -1;
	if (fds[1] != -1 && close(fds[1]) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			fds[1], info->token_list->str, strerror(errno));
	}
	fds[1] = -1;
}

void	check_path_validity(char **args, t_minishell *info)
{
	if (args[0][0] == '/' || ft_strncmp(args[0], "./", 2) == 0 || \
		ft_strncmp(args[0], "../", 3) == 0)
	{
		execve(args[0], args, info->envp);
		info->last_exit_status = errno;
		if (errno == ENOENT)
			ft_fprintf(STDERR_FILENO, "%s: command not found\n", args[0]);
		else
			ft_fprintf(STDERR_FILENO, "execve: %s\n", strerror(errno));
	}
	else
	{
		info->last_exit_status = ENOENT;
		ft_fprintf(STDERR_FILENO, "%s: command not found\n", args[0]);
	}
}
