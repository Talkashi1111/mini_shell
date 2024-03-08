/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:01:11 by tkashi            #+#    #+#             */
/*   Updated: 2023/10/29 16:11:40 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

char	*is_known(int num)
{
	char	*ret;

	ret = NULL;
	if (num == -2147483648)
		ret = ft_strdup("-2147483648");
	else if (num == 0)
		ret = ft_strdup("0");
	return (ret);
}

void	reverse_str(int start, int end, char *str)
{
	char	temp;

	while (start < end)
	{
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

void	apply_itoa(int num, char *str)
{
	int	end;
	int	start;

	end = 0;
	start = 0;
	if (num < 0)
	{
		num *= -1;
		str[end] = '-';
		end++;
		start++;
	}
	while (num > 0)
	{
		str[end++] = num % 10 + '0';
		num = num / 10;
	}
	str[end] = '\0';
	end--;
	reverse_str(start, end, str);
}

int	calc_size(int n)
{
	int	size;

	size = 0;
	if (n < 0)
	{
		size++;
		n *= -1;
	}
	while (n > 0)
	{
		size++;
		n /= 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char	*ret;
	int		size;

	if (n == 0 || n == -2147483648)
	{
		ret = is_known(n);
		if (!ret)
			return (NULL);
		else
			return (ret);
	}
	else
	{
		size = calc_size(n);
		ret = (char *)malloc((size + 1) * sizeof(char));
		if (!ret)
			return (NULL);
		apply_itoa(n, ret);
	}
	return (ret);
}
