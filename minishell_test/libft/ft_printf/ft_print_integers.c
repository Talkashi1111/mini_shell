/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_integers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 19:07:54 by achappui          #+#    #+#             */
/*   Updated: 2023/11/10 15:46:12 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	calcul_integer_options(char len, char prefix_len, t_options *o)
{
	o->width -= len + prefix_len;
	o->precision -= len;
	if (o->precision < 0)
		o->precision = 0;
	o->width -= o->precision;
	if (o->width < 0)
		o->width = 0;
	if (!(o->flags & F_MINUS) && !(o->flags & F_PRECISION) && \
		(o->flags & F_ZERO))
	{
		o->precision = o->width;
		o->width = 0;
	}
}

int	ft_print_uint(unsigned long long n, const char *base, char blen, \
					t_options *o)
{
	char	tab[MAX_OCTAL_LLLEN];
	short	i;

	i = sizeof(tab);
	if (o->spe == 'p')
		o->flags |= F_HASHTAG;
	else if (n == 0 && o->spe != 'd' && o->spe != 'i')
		o->flags &= ~F_HASHTAG;
	while (n > 0 || (i == sizeof(tab) && !(o->flags & F_PRECISION)))
	{
		tab[--i] = base[(n % blen)];
		n /= blen;
	}
	n = sizeof(tab) - i;
	base += blen;
	calcul_integer_options(n, *base * (o->flags & F_HASHTAG), o);
	if ((!(o->flags & F_MINUS) && ft_putchars(o->width, ' ') == -1) || \
		((o->flags & F_HASHTAG) && write(1, base + 1, *base) != *base) || \
		ft_putchars(o->precision, '0') == -1 || \
		write(1, tab + i, n) != (ssize_t)n || \
		((o->flags & F_MINUS) && ft_putchars(o->width, ' ') == -1))
		return (-1);
	return (o->width + o->precision + n + *base * (o->flags & F_HASHTAG));
}

int	ft_print_sint(long long n, t_options *o)
{
	o->flags |= F_HASHTAG;
	if (n < 0)
		return (ft_print_uint(-n, "0123456789\001-", 10, o));
	else if (o->flags & F_PLUS)
		return (ft_print_uint(n, "0123456789\001+", 10, o));
	else if (o->flags & F_SPACE)
		return (ft_print_uint(n, "0123456789\001 ", 10, o));
	else
		return (ft_print_uint(n, "0123456789\000#", 10, o));
}
