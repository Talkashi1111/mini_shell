/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:48:07 by achappui          #+#    #+#             */
/*   Updated: 2024/04/16 20:32:12 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int    save_std_streams(int saved_std[2], t_minishell *info)
{
    saved_std[0] = dup(0);
    if (saved_std[0] == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    saved_std[1] = dup(1);
    if (saved_std[1] == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    return (OK);
}

int    restore_std_streams(int saved_std[2], t_minishell *info)
{
    if (dup2(saved_std[0], 0) == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    if (dup2(saved_std[1], 1) == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    return (OK);
}

void	get_last_stdin_stdout(t_token_list *redi, t_token_list **last_stdin, t_token_list **last_stdout)
{
	*last_stdin = NULL;
	*last_stdout = NULL;
	while (redi)// token_size >= 2 ?? token_size --?
	{
		if (redi->type == STDIN || redi->type == STDIN_HEREDOC)
			*last_stdin = redi;
		else if (redi->type == STDOUT || redi->type == STDOUT_APPEND)
			*last_stdout = redi;
		redi = redi->next;
		redi = redi->next;
	}
}

#include <stdio.h>
int	apply_redirections(t_cmd *cmd, t_minishell *info, t_token_list *redi) //checker les erreurs
{
	int				fd;
	t_token_list	*last_stdin;
	t_token_list	*last_stdout;

	get_last_stdin_stdout(redi, &last_stdin, &last_stdout);//you are pointing at this stage on last_stdin ->str = "<" and trying to open it?
	if (last_stdin && last_stdin->type == STDIN)
	{
		fd = open(last_stdin->next->str, O_RDONLY, 0777);
		if (fd < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "open: %s\n", strerror(errno));
			return (info->last_exit_status);
		}
		if(dup2(fd, 0) == -1)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
			close(fd); //erreur ?
			return (info->last_exit_status);
		}
		close(fd); //erreur ?
	}
	else if (last_stdin && last_stdin->type == STDIN_HEREDOC)
	{
		if (dup2(cmd->heredoc_pipe[0], 0) == -1)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
			return (info->last_exit_status);
		}
	}
	if (last_stdout && last_stdout->type == STDOUT)
	{
		fd = open(last_stdout->next->str, O_WRONLY | O_CREAT, 0777);
		if (fd < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "open: %s\n", strerror(errno));
			return (info->last_exit_status);
		}
		if(dup2(fd, 1) == -1)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
			close(fd); //erreur ?
			return (info->last_exit_status);
		}
		close(fd); //erreur ?
	}
	else if (last_stdout && last_stdout->type == STDOUT_APPEND)
	{
		fd = open(last_stdout->next->str, O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd < 0)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "open: %s\n", strerror(errno));
			return (info->last_exit_status);
		}
		if(dup2(fd, 1) == -1)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
			close(fd); //erreur ?
			return (info->last_exit_status);
		}
		close(fd); //erreur ?
	}
	return (OK);
}

int	check_redirections(t_node *node, t_minishell *info)
{
	t_token_list	*index;

	index = node->redi;
	info->last_exit_status = OK;
	if (index)
	{
		while (index->next)
		{
			if (((index->type >= STDIN && index->type <= STDOUT_APPEND) && \
			(index->next->type >= STDIN && index->next->type <= STDOUT_APPEND)) ||
			(index->type == WORD && index->next->type == WORD))
				info->last_exit_status = 1;
			index = index->next;
		}
		if (index->type >= STDIN && index->type <= STDOUT_APPEND)
			info->last_exit_status = 1;
	}
	if (info->last_exit_status != OK)
		ft_fprintf(STDERR_FILENO, "bash: ambigous redirection\n");
	return (info->last_exit_status);
}