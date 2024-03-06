/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:27:03 by achappui          #+#    #+#             */
/*   Updated: 2023/11/23 10:01:48 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char			*s_dup;
	unsigned int	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	s_dup = (char *)malloc((len + 1) * sizeof(char));
	if (!s_dup)
		return (NULL);
	while (*s != '\0')
		*s_dup++ = *s++;
	*s_dup = '\0';
	s_dup -= len;
	return (s_dup);
}
