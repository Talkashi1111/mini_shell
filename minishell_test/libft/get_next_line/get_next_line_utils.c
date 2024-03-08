/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 18:29:52 by achappui          #+#    #+#             */
/*   Updated: 2024/01/17 16:21:59 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_block	*ft_read_next_block(int fd)
{
	t_block	*new_block;
	ssize_t	read_bytes;

	new_block = (t_block *)malloc(1 * sizeof(t_block));
	if (new_block == NULL)
		return (NULL);
	read_bytes = read(fd, new_block->buffer, BUFFER_SIZE);
	if (read_bytes == -1)
	{
		free(new_block);
		return (NULL);
	}
	new_block->buffer[read_bytes] = '\0';
	new_block->next = NULL;
	return (new_block);
}

void	ft_freeblocks(t_block **block)
{
	t_block	*next;

	while (*block != NULL)
	{
		next = (*block)->next;
		free(*block);
		*block = next;
	}
	*block = NULL;
}
