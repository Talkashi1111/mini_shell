/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 11:51:19 by tkashi            #+#    #+#             */
/*   Updated: 2023/10/26 12:03:36 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_isspace(char ch)
{
	if (ch == ' ' || ch == '\t' || ch == '\n')
		return (1);
	else if (ch == '\f' || ch == '\v' || ch == '\r')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	count;
	int	ret;

	while (*str != '\0' && ft_isspace(*str))
	{
		str++;
	}
	count = 0;
	if ((*str == '+' || *str == '-') && *str != '\0')
	{
		if (*str == '-')
			count++;
		str++;
	}
	ret = 0;
	while (ft_isdigit(*str) && *str && (*str != '+' && *str != '-'))
	{
		ret = ret * 10 + (*str - '0');
		str++;
	}
	if (count % 2 == 1)
		ret *= -1;
	return (ret);
}
