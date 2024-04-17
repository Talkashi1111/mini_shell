/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:51:11 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/17 15:56:44 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_heredoc(char *eof, int saved_streams[2], t_minishell *info, int fd)
{
	char	*line;
	unsigned int eof_len;

	eof_len = ft_strlen(eof);
	while (TRUE)
	{
		line = get_next_line(saved_streams[0]);
		if (!line || ((ft_strncmp(line, eof, eof_len) == 0) && line[eof_len] == '\n'))
		{
			info->last_exit_status = errno;
			if (line)
				free(line);
			break ;
		}
		write(saved_streams[1], ">", 1);
		if (write(fd, line, ft_strlen(line)) < 0)
		{
			ft_fprintf(STDERR_FILENO, "write: %s\n", strerror(errno));
			free(line);
			return ;
		}
		free(line);
	}
}

int	fill_heredoc(int saved_streams[2], char *eof, t_minishell *info)
{
	int	fd;

	fd = open(".heredoc", O_WRONLY | O_TRUNC | O_APPEND, 0777); //pour les perm autre chose ?
	if (fd == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "open: %s\n", strerror(errno));
		return (info->last_exit_status);
	}
	ft_fprintf(saved_streams[1], ">");
	//write(saved_streams[1], ">", 1); //checker le write ? //PROBLEME DE VITESSE d'APPARITION DU >
	print_heredoc(eof, saved_streams, info, fd);
	if (close(fd) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "close: %s\n", strerror(errno));
	}
	return (OK);
}