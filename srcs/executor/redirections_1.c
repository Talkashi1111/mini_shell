/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:48:07 by achappui          #+#    #+#             */
/*   Updated: 2024/04/17 12:31:48 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int    save_std_streams(int saved_streams[2], t_minishell *info)
{
    saved_streams[0] = dup(0);
    if (saved_streams[0] == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    saved_streams[1] = dup(1);
    if (saved_streams[1] == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    return (OK);
}

int    restore_std_streams(int saved_streams[2], t_minishell *info)
{
    if (dup2(saved_streams[0], STDIN_FILENO) == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    if (dup2(saved_streams[1], STDOUT_FILENO) == -1)
    {
        info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
        return (info->last_exit_status);
    }
    return (OK);
}

int	redirect(int redi_fd, char *path, int flags, t_minishell *info)
{
	int	fd;

	fd = open(path, flags, 0777); //pour les perm autre chose ?
	if (fd == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "open: %s\n", strerror(errno)); //ces erreur doivent ecrire sur saved_streams toutes les erreurs apres le save enfaite
		return (info->last_exit_status);
	}
	if(dup2(fd, redi_fd) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "dup2: %s\n", strerror(errno));
		if (close(fd) == -1)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
		}
		return (info->last_exit_status);
	}
	if (close(fd) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
	}
	return (OK);
}

int	fill_heredoc(int saved_streams[2], char *eof, t_minishell *info)
{
	char			*line;
	unsigned int	eof_len;
	int				fd;

	fd = open(".heredoc", O_WRONLY | O_TRUNC | O_APPEND, 0777); //pour les perm autre chose ?
	if (fd == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "open: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	eof_len = ft_strlen(eof);
	write(saved_streams[1], ">", 1); //checker le write ? //PROBLEME DE VITESSE d'APPARITION DU >
	line = get_next_line(saved_streams[0]);
	if (!line)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "read: %s\n", strerror(errno));
		ft_exit(NULL, info);
	}
	write(fd, line, ft_strlen(line)); //checker le write
	while (!(ft_strncmp(line, eof, eof_len) == 0 && line[eof_len] == '\n'))
	{
		free(line);
		write(saved_streams[1], ">", 1); //checker le write ?
		line = get_next_line(saved_streams[0]);
		if (!line)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "read: %s\n", strerror(errno));
			ft_exit(NULL, info);
		}
		write(fd, line, ft_strlen(line)); //checker le write
	}
	free(line);
	if (close(fd) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
	}
	return (OK);
}
#include <stdio.h>// ca sert  a quoi???

int	apply_redirections(int saved_streams[2], t_token_list *redi, t_minishell *info)
{
	while (redi)
	{
		if (redi->type == STDIN && redirect(0, redi->next->str, O_RDONLY, info) != OK)
			return (info->last_exit_status);
		else if (redi->type == STDIN_HEREDOC && (fill_heredoc(saved_streams, redi->next->str, info) != OK || redirect(STDIN_FILENO, ".heredoc", O_RDONLY, info) != OK))
			return (info->last_exit_status);
		else if (redi->type == STDOUT && redirect(STDOUT_FILENO, redi->next->str, O_CREAT | O_WRONLY | O_TRUNC, info) != OK)
			return (info->last_exit_status);
		else if (redi->type == STDOUT_APPEND && redirect(1, redi->next->str, O_CREAT | O_WRONLY | O_APPEND, info) != OK)
			return (info->last_exit_status);
		redi = redi->next;
		redi = redi->next;
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