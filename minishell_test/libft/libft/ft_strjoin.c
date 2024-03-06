/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:27:03 by achappui          #+#    #+#             */
/*   Updated: 2023/11/23 10:01:55 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	uintptr_t		str;
	unsigned int	i;

	i = 0;
	str = 0;
	while (s1[i] != '\0')
		i++;
	while (s2[str++] != '\0')
		i++;
	str = (uintptr_t)malloc((i + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (*s1 != '\0')
		*(char *)str++ = *s1++;
	while (*s2 != '\0')
		*(char *)str++ = *s2++;
	*(char *)str = '\0';
	str -= i;
	return ((char *)str);
}
