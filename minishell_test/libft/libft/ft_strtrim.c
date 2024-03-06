/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:27:03 by achappui          #+#    #+#             */
/*   Updated: 2023/11/23 10:33:15 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	ft_in_set(char const *set, char character)
{
	while (*set != '\0')
	{
		if (*set == character)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s2;
	size_t	len;

	s2 = (char *)s1;
	while (*s1 != '\0')
		s1++;
	while (*s2 != '\0' && ft_in_set(set, *s2))
		s2++;
	if (s1 != s2)
		while (s1 != s2 && (ft_in_set(set, *s1) || *s1 == '\0'))
			s1--;
	if (*s1 == '\0' && *s2 == '\0')
		len = 0;
	else
		len = s1 - s2 + 1;
	s2 = (char *)malloc((len + 1) * sizeof(char));
	if (s2 == NULL)
		return (NULL);
	s2[len] = '\0';
	while (len-- > 0)
		s2[len] = *s1--;
	return (s2);
}
