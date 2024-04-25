/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:48:07 by achappui          #+#    #+#             */
/*   Updated: 2024/04/25 13:49:42 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_std_streams(t_minishell *info)
{
	info->saved_streams[0] = dup(0);
	if (info->saved_streams[0] == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "dup: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	info->saved_streams[1] = dup(1);
	if (info->saved_streams[1] == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "dup: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	return (OK);
}

int	restore_std_streams(int saved_streams[2], t_minishell *info)
{
	if (dup2(saved_streams[0], 0) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "dup2: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	if (dup2(saved_streams[1], 1) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "dup2: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	return (OK);
}

int	redirect(int redi_fd, char *path, int flags, t_minishell *info)
{
	int	fd;

	fd = open(path, flags, 0777);
	if (fd == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "open: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	if (dup2(fd, redi_fd) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "dup2: %s\n", strerror(errno));
		close_saved_streams(info, fd);
		return (info->last_exit_status);
	}
	close_saved_streams(info, fd);
	return (OK);
}

int	apply_redirections(t_token_list *redi, t_minishell *info)
{
	while (redi)
	{
		if (redi->type == STDIN
			&& redirect(STDIN_FILENO, redi->next->str, O_RDONLY, info) != OK)
			return (info->last_exit_status);
		else if (redi->type == STDIN_HEREDOC
			&& redirect(STDIN_FILENO, redi->next->str, O_RDONLY, info) != OK)
			return (info->last_exit_status);
		else if (redi->type == STDOUT
			&& redirect(STDOUT_FILENO, redi->next->str,
				O_CREAT | O_WRONLY | O_TRUNC, info) != OK)
			return (info->last_exit_status);
		else if (redi->type == STDOUT_APPEND
			&& redirect(STDOUT_FILENO, redi->next->str,
				O_CREAT | O_WRONLY | O_APPEND, info) != OK)
			return (info->last_exit_status);
		redi = redi->next;
		redi = redi->next;
	}
	return (OK);
}

int	check_redirections(t_node *node, t_minishell *info)
{
	t_token_list	*index;
	int				ambiguity;

	index = node->redi;
	ambiguity = OK;
	if (index)
	{
		while (index->next)
		{
			if (((index->type >= STDIN && index->type <= STDOUT_APPEND) && \
			(index->next->type >= STDIN && index->next->type <= STDOUT_APPEND))
				|| (index->type == WORD && index->next->type == WORD))
				ambiguity = AMBIGOUS_REDI_ERROR;
			index = index->next;
		}
		if (index->type >= STDIN && index->type <= STDOUT_APPEND)
			ambiguity = AMBIGOUS_REDI_ERROR;
	}
	if (ambiguity != OK)
		ft_fprintf(info->saved_streams[1], "Ambigous redirection !\n");
	info->last_exit_status = ambiguity;
	return (info->last_exit_status);
}
