/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 18:29:56 by achappui          #+#    #+#             */
/*   Updated: 2024/01/12 10:11:13 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 3
# endif

typedef struct s_block
{
	char			buffer[BUFFER_SIZE + 1];
	struct s_block	*next;
}	t_block;

typedef struct s_sinfo
{
	unsigned int	sindex;
	struct s_block	sblock;
}	t_sinfo;

t_block	*ft_read_next_block(int fd);
void	ft_freeblocks(t_block **blk);

char	*get_next_line(int fd);

#endif