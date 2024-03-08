/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 18:29:54 by achappui          #+#    #+#             */
/*   Updated: 2024/01/17 16:23:02 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_fill_line(char *line, t_sinfo *si, unsigned int index)
{
	t_block	*blocks;

	blocks = &si->sblock;
	while (1)
	{
		if (blocks->buffer[index] == '\0')
		{
			blocks = blocks->next;
			index = 0;
			if (blocks->buffer[0] == '\0')
			{
				si->sindex = BUFFER_SIZE;
				return ;
			}
		}
		if (blocks->buffer[index] == '\n')
		{
			*line = '\n';
			si->sindex = index + 1;
			while (++index < BUFFER_SIZE)
				si->sblock.buffer[index] = blocks->buffer[index];
			return ;
		}
		*line++ = blocks->buffer[index++];
	}
}

int	ft_get_line_length(int fd, t_block *block, unsigned int index)
{
	int	len;

	len = 0;
	while (1)
	{
		if (block->buffer[index] == '\0')
		{
			block->next = ft_read_next_block(fd);
			if (!block->next)
				return (-1);
			block = block->next;
			index = 0;
			if (block->buffer[0] == '\0')
				return (len);
		}
		if (block->buffer[index] == '\n')
			return (len + 1);
		len++;
		index++;
	}
}

char	*get_next_line(int fd)
{
	static t_sinfo	si = {.sindex = BUFFER_SIZE, .sblock.next = NULL, \
							.sblock.buffer = {[BUFFER_SIZE] = '\0'}};
	char			*line;
	int				len;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	len = ft_get_line_length(fd, &si.sblock, si.sindex);
	if (len > 0)
	{
		line = (char *)malloc((len + 1) * sizeof(char));
		if (line)
		{
			line[len] = '\0';
			ft_fill_line(line, &si, si.sindex);
		}
	}
	else
		si.sindex = BUFFER_SIZE;
	ft_freeblocks(&si.sblock.next);
	if (len == -1)
		return ((void *)-1);
	return (line);
}
