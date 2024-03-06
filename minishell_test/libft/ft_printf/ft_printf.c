/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 09:01:12 by achappui          #+#    #+#             */
/*   Updated: 2023/11/23 10:38:23 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	choose_printer(va_list args, t_options *o)
{
	if (o->spe == '%')
		return (ft_print_char('%', o));
	else if (o->spe == 'c')
		return (ft_print_char(va_arg(args, int), o));
	else if (o->spe == 's')
		return (ft_print_str(va_arg(args, const char *), o));
	else if (o->spe == 'd' || o->spe == 'i')
		return (ft_print_sint(va_arg(args, int), o));
	else if (o->spe == 'u')
		return (ft_print_uint(va_arg(args, unsigned int), BASE_TEN, 10, o));
	else if (o->spe == 'x')
		return (ft_print_uint(va_arg(args, unsigned int), LOW_HEX, 16, o));
	else if (o->spe == 'X')
		return (ft_print_uint(va_arg(args, unsigned int), UPP_HEX, 16, o));
	else if (o->spe == 'p')
		return (ft_print_uint(va_arg(args, uintptr_t), LOW_HEX, 16, o));
	return (-1);
}

static char	percent_state(const char **str, t_count *c, t_options *o, \
							va_list args)
{
	int	tmp_len;

	c->options_len = update_options(*str, o);
	if (c->options_len == -1)
		return (0);
	if (write(1, *str - c->saved_len - 1, c->saved_len) != c->saved_len)
		return (-1);
	tmp_len = choose_printer(args, o);
	if (tmp_len == -1)
		return (-1);
	c->printed_chars += c->saved_len + tmp_len;
	c->saved_len = 0;
	*str += c->options_len;
	return (0);
}

int	ft_printf(const char *str, ...)
{
	va_list		args;
	t_options	o;
	t_count		c;

	if (!str)
		return (-1);
	va_start(args, str);
	c.saved_len = 0;
	c.printed_chars = 0;
	while (*str != '\0')
	{
		if (*str++ == '%')
		{
			if (percent_state(&str, &c, &o, args) == -1)
				return (-1);
		}
		else
			c.saved_len++;
	}
	va_end(args);
	if (write(1, str - c.saved_len, c.saved_len) != c.saved_len)
		return (-1);
	return (c.printed_chars + c.saved_len);
}
