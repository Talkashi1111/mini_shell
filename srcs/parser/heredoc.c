/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:49:43 by achappui          #+#    #+#             */
/*   Updated: 2024/04/19 17:09:46 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_expander_error(char *str, int to_close, t_minishell *info)
{
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
	t_token_list	*new_heredoc_token;

	*path = ft_itoa_heredoc(file_no);
	if (!*path)
		return (heredoc_expander_error("malloc: %s\n", -1, info));
	new_heredoc_token = new_token(*path, HEREDOC);
	if (!new_heredoc_token)
	{
		free(*path);
		return (heredoc_expander_error("malloc: %s\n", -1, info));
	}
	*fd = open(*path, O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0777);
	if (*fd == -1)
	{
		free(new_heredoc_token);
		free(*path);
		return (heredoc_expander_error("open: %s\n", -1, info));
	}
	add_back_token_list(&info->token_list, new_heredoc_token);
	if (path)
		free(*path);
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
			return (heredoc_expander_error("read: %s\n", fd, info));
		if ((write(fd, line, ft_strlen(line)) == -1 && ft_free(line)) || \
			(write(fd, "\n", 1) == -1 && ft_free(line)))
			return (heredoc_expander_error("write: %s\n", fd, info));
		free(line);
	}
	free(line);
	heredoc_expander_end(fd, eof, path, info);
	return (OK);
}
