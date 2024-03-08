/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_u.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 20:20:45 by tkashi            #+#    #+#             */
/*   Updated: 2023/11/05 20:21:09 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include "ft_printf.h"

void	ft_print_u(int fd, unsigned int n, t_count *stcount)
{
	if (n < 10)
	{
		ft_print_c(fd, n + '0', stcount);
		return ;
	}
	ft_print_u(fd, n / 10, stcount);
	ft_print_c(fd, n % 10 + '0', stcount);
}
