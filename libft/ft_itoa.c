/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 12:01:11 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 14:50:24 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_itoa(int n)
{
	long long	tab;
	short		i;

	tab = n;
	i = 1;
	while ((tab >= 10 || tab < 0) && i++)
		tab /= 10;
	tab = (long long)malloc((i + 1) * sizeof(char));
	if (!tab)
		return (NULL);
	((char *)tab)[i] = '\0';
	((char *)tab)[0] = '+';
	if (n < 0)
		((char *)tab)[0] = '-';
	while (n < 0)
	{
		((char *)tab)[--i] = -(n % 10) + 48;
		n /= 10;
	}
	while (n > 0 || (i == 1 && ((char *)tab)[0] != '-'))
	{
		((char *)tab)[--i] = n % 10 + 48;
		n /= 10;
	}
	return ((char *)tab);
}
