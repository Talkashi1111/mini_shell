/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:27:03 by achappui          #+#    #+#             */
/*   Updated: 2023/11/23 10:12:16 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
