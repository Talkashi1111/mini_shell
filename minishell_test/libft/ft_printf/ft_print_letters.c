/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_letters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:57:05 by achappui          #+#    #+#             */
/*   Updated: 2023/11/10 15:45:55 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchars(int n, char c)
{
	char	buffer[BUFFER_SIZE];
	short	i;

	if (n <= 0)
		return (0);
	i = 0;
	while (i < BUFFER_SIZE)
		buffer[i++] = c;
	while (n > BUFFER_SIZE)
	{
		if (write(1, buffer, BUFFER_SIZE) != BUFFER_SIZE)
			return (-1);
		n -= BUFFER_SIZE;
	}
	if (n > 0)
	{
		if (write(1, buffer, n) != n)
			return (-1);
		n -= n;
	}
	return (0);
}

int	ft_print_char(unsigned char c, t_options *o)
{
	if (o->width)
		o->width--;
	if ((!(o->flags & F_MINUS) && ft_putchars(o->width, ' ') == -1) || \
		write(1, &c, 1) != 1 || \
		((o->flags & F_MINUS) && ft_putchars(o->width, ' ') == -1))
		return (-1);
	return (o->width + 1);
}

int	ft_print_str(const char *s, t_options *o)
{
	size_t	len;

	len = 0;
	if (!s)
		s = "(null)";
	while (*s != '\0')
	{
		s++;
		len++;
	}
	s -= len;
	if ((o->flags & F_PRECISION) && (size_t)o->precision < len)
		len = o->precision;
	if ((size_t)o->width > len)
		o->width -= len;
	else
		o->width = 0;
	if ((!(o->flags & F_MINUS) && ft_putchars(o->width, ' ') == -1) || \
		write(1, s, len) != (ssize_t)len || \
		((o->flags & F_MINUS) && ft_putchars(o->width, ' ') == -1))
		return (-1);
	return (o->width + len);
}
