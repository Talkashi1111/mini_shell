/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_s.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 20:12:47 by tkashi            #+#    #+#             */
/*   Updated: 2023/11/05 20:14:16 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include "ft_printf.h"

void	ft_print_s(int fd, char *s, t_count *stcount)
{
	int	res;

	if (!s)
	{
		res = write(fd, "(null)", 6 * sizeof(char));
		if (res < 0)
			stcount->error += 1;
		else
			stcount->count += res;
		return ;
	}
	while (*s != '\0' && !stcount->error)
	{
		ft_print_c(fd, *s, stcount);
		s++;
	}
}
