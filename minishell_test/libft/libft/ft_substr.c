/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:27:03 by achappui          #+#    #+#             */
/*   Updated: 2023/11/01 10:29:46 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_str;

	while (*s != '\0' && start > 0)
	{
		s++;
		start--;
	}
	if (start > 0)
		start = 0;
	while (*s != '\0' && start < len)
	{
		s++;
		start++;
	}
	sub_str = (char *)malloc((start + 1) * sizeof(char));
	if (sub_str == NULL)
		return (NULL);
	*(sub_str + start) = '\0';
	while (start > 0)
		*(sub_str + --start) = *(--s);
	return (sub_str);
}
