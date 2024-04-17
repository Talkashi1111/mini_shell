/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:51:11 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/17 13:57:51 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	//write(saved_streams[1], ">", 1); //checker le write ? //PROBLEME DE VITESSE d'APPARITION DU >
	while (TRUE)
	{
		write(saved_streams[1], ">", 1);
		line = get_next_line(saved_streams[0]);
		if (!line || ((ft_strncmp(line, eof, eof_len) == 0) && line[eof_len] == '\n'))
		{
			info->last_exit_status = errno;
			if (!line)
			{
				ft_fprintf(STDERR_FILENO, "read: %s\n", strerror(errno));
				ft_exit(NULL, info);
			}
			free(line);
			break ;
		}

		if (write(fd, line, ft_strlen(line)) < 0)
		{
			ft_fprintf(STDERR_FILENO, "write: %s\n", strerror(errno));
			free(line);
			ft_exit(NULL, info);
		}
		free(line);
	}
	/* line = get_next_line(saved_streams[0]);
	if (!line)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "read: %s\n", strerror(errno));
		ft_exit(NULL, info);
	}
	//write(fd, line, ft_strlen(line)); //checker le write
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
	free(line); */
	if (close(fd) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
	}
	return (OK);
}