/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 19:52:30 by tkashi            #+#    #+#             */
/*   Updated: 2023/11/05 19:55:08 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

void	ft_print_p(int fd, unsigned long addr, t_count *stcount)
{
	int				i;
	unsigned long	remind;
	char			hexa[2 * sizeof(unsigned long)];

	if (addr == 0)
	{
		ft_print_s(fd, "0x0", stcount);
		return ;
	}
	i = 0;
	while (addr > 0)
	{
		remind = addr % 16;
		if (remind < 10)
			hexa[i++] = '0' + remind;
		else
			hexa[i++] = 'a' + remind - 10;
		addr = addr / 16;
	}
	ft_print_s(fd, "0x", stcount);
	i--;
	while (i >= 0 && !stcount->error)
		ft_print_c(fd, hexa[i--], stcount);
}
