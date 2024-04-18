/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:49:43 by achappui          #+#    #+#             */
/*   Updated: 2024/04/18 17:14:05 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_expander_error(char *str, char *to_free, int to_close, t_minishell *info)
{
	if (to_free)
		free(to_free);
	if (to_close != -1)
	{
		if (close(to_close) == -1)
		{
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
		}
	}
	info->last_exit_status = errno;
	ft_fprintf(STDERR_FILENO, str, strerror(errno));
	return (info->last_exit_status);
}

int	heredoc_expander_start(uintptr_t file_no, char **path, int *fd, t_minishell *info)
{
	*path = ft_itoa_heredoc(file_no);
	if (!*path)
		return (heredoc_expander_error("malloc: %s\n", NULL, -1, info));
	*fd = open(*path, O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0777);
	if (*fd == -1)
		return (heredoc_expander_error("open: %s\n", *path, -1, info));
	return (OK);
}

void	heredoc_expander_end(int fd, char **eof, char *path, t_minishell *info)
{
	if (close(fd) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
	}
	free(*eof);
	*eof = path;
}

int	ft_free(char *line)
{
	free(line);
	return (1);
}

int	heredoc_expander(uintptr_t file_no, char **eof, t_minishell *info)
{
	char			*path;
	char			*line;
	unsigned int	eof_len;
	int				fd;

	if (heredoc_expander_start(file_no, &path, &fd, info) != OK)
		return (info->last_exit_status);
	eof_len = ft_strlen(*eof);
	while (1)
	{
		line = readline(">");
		if (ft_strncmp(line, *eof, eof_len) == 0 && line[eof_len] == '\0')
			break ;
		if (!line)
			return (heredoc_expander_error("read: %s\n", path, fd, info));
		if ((write(fd, line, ft_strlen(line)) == -1 && ft_free(line)) || \
			(write(fd, "\n", 1) == -1 && ft_free(line)))
			return (heredoc_expander_error("write: %s\n", path, fd, info));
		free(line);
	}
	free(line);
	heredoc_expander_end(fd, eof, path, info);
	return (OK);
}