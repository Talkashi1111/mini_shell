/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 19:31:25 by tkashi            #+#    #+#             */
/*   Updated: 2023/11/05 19:33:19 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include "ft_printf.h"

void	ft_print_x(int fd, unsigned int x, char flag, t_count *stcount)
{
	int				i;
	char			hexa[2 * sizeof(int)];
	unsigned int	remind;

	if (x == 0)
	{
		ft_print_c(fd, '0', stcount);
		return ;
	}
	i = 0;
	while (x > 0)
	{
		remind = x % 16;
		if (remind < 10)
			hexa[i++] = '0' + remind;
		else if ('x' == flag)
			hexa[i++] = 'a' + remind - 10;
		else
			hexa[i++] = 'A' + remind - 10;
		x = x / 16;
	}
	i--;
	while (i >= 0 && !stcount->error)
		ft_print_c(fd, hexa[i--], stcount);
}
