/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:46:54 by achappui          #+#    #+#             */
/*   Updated: 2024/04/16 22:04:08 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h> //linux
#include <readline/history.h> //linux

t_token_list *get_last_heredoc(t_token_list *redi)
{
	t_token_list	*last_heredoc;

	last_heredoc = NULL;
	while (redi)
	{
		if (redi->type == STDIN_HEREDOC)
			last_heredoc = redi;
		redi = redi->next;
		redi = redi->next;
	}
	return (last_heredoc);
}

#include <stdio.h>
int	run_potential_heredocs(t_token_list *redi, t_minishell *info, t_cmd *cmd)
{
	int				pid;
	t_token_list	*last_heredoc;
	unsigned int	delim_len;
	char			*line;

	last_heredoc = get_last_heredoc(redi);
	if (last_heredoc == NULL) //donc si il n'y a pas de heredoc
		return (OK);
	if (pipe(cmd->heredoc_pipe) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "pipe: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	pid = fork();
	if (pid == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "fork: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	if (pid == 0)
	{
		free_args(cmd->args);
		// if (dup2(cmd->heredoc_pipe[1], 1) == -1)
		// {
		// 	info->last_exit_status = errno;
		// 	ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
		// 	ft_close_size_2(cmd->heredoc_pipe, info);
		// 	ft_exit(NULL, info);
		// }
		// ft_close_size_2(cmd->heredoc_pipe, info);
		while (redi)
		{
			if (redi->type == STDIN_HEREDOC && redi != last_heredoc)
			{
				delim_len = ft_strlen(redi->next->str);
				write(1, ">", 1);
				line = get_next_line(0);
				if (!line)
				{
					info->last_exit_status = errno;
					ft_fprintf(STDERR_FILENO, "get_next_line: %s\n", strerror(errno));
					ft_close_size_2(cmd->heredoc_pipe, info);
					ft_exit(NULL, info);
				}
				while (!(ft_strncmp(line, redi->next->str, delim_len) == 0 && line[delim_len] == '\n'))
				{
					free(line);
					write(1, ">", 1);
					line = get_next_line(0);
					if (!line)
					{
						info->last_exit_status = errno;
						ft_fprintf(STDERR_FILENO, "get_next_line: %s\n", strerror(errno));
						ft_close_size_2(cmd->heredoc_pipe, info);
						ft_exit(NULL, info);
					}
				}
				free(line);
			}
			else if (redi->type == STDIN_HEREDOC && redi == last_heredoc)
			{
				delim_len = ft_strlen(redi->next->str);
				write(1, ">", 1);
				line = get_next_line(0);
				if (!line)
				{
					info->last_exit_status = errno;
					ft_fprintf(STDERR_FILENO, "get_next_line: %s\n", strerror(errno));
					ft_close_size_2(cmd->heredoc_pipe, info);
					ft_exit(NULL, info);
				}
				while (!(ft_strncmp(line, redi->next->str, delim_len) == 0 && line[delim_len] == '\n')) //est-il possible d'entrer des \n dans le terminal pour un heredoc ?
				{
					if (write(cmd->heredoc_pipe[1], line, ft_strlen(line)) == -1)
					{
						info->last_exit_status = errno;
						ft_fprintf(STDERR_FILENO, "write: %s\n", strerror(errno));
						free(line);
						ft_close_size_2(cmd->heredoc_pipe, info);
						ft_exit(NULL, info);
					}
					free(line);
					write(1, ">", 1);
					line = get_next_line(0);
					if (!line)
					{
						info->last_exit_status = errno; //ya pas de errno pour get next line
						ft_fprintf(STDERR_FILENO, "get_next_line: %s\n", strerror(errno));
						ft_close_size_2(cmd->heredoc_pipe, info);
						ft_exit(NULL, info);
					}
				}
				free(line);
			}
			redi = redi->next;
			redi = redi->next;
		}
		ft_close_size_2(cmd->heredoc_pipe, info);
		ft_exit(NULL, info);
	}
	cmd->pid = pid;
	return (OK);
}
#include <stdio.h>
int	wait_potential_heredocs(int pid, t_minishell *info)
{
	int	status;

	if (pid == -1)
		return (OK);
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
	return (OK);
}