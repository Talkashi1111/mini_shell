/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_options.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 19:11:11 by achappui          #+#    #+#             */
/*   Updated: 2023/11/23 10:36:34 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	in_flags(char c, const char *flags)
{
	char	i;

	i = 1;
	while (*flags != '\0')
	{
		if (*flags == c)
			return (i);
		flags++;
		i *= 2;
	}
	return (0);
}

static void	get_flags_width_precision(const char **str, t_options *o)
{
	const char	*flags;
	char		i;

	flags = FLAGS;
	i = in_flags(**str, flags);
	while (i)
	{
		o->flags |= i;
		i = in_flags(*(++*str), flags);
	}
	while (**str >= '0' && **str <= '9')
		o->width = o->width * 10 + *(*str)++ - 48;
	if (**str == '.')
		o->flags |= F_PRECISION;
	if (o->flags & F_PRECISION)
		while (*(++(*str)) >= '0' && **str <= '9')
			o->precision = o->precision * 10 + **str - 48;
}

static char	get_specifier(const char **str, t_options *o)
{
	const char	*specifiers;

	specifiers = SPECIFIERS;
	while (*specifiers != '\0')
	{
		if (*specifiers == **str)
		{
			o->spe = *specifiers;
			(*str)++;
			return (1);
		}
		specifiers++;
	}
	return (0);
}

int	update_options(const char *str, t_options *o)
{
	const char	*old_str;

	old_str = str;
	o->flags = 0;
	o->precision = 0;
	o->spe = 0;
	o->width = 0;
	get_flags_width_precision(&str, o);
	if (get_specifier(&str, o))
		return (str - old_str);
	return (-1);
}
