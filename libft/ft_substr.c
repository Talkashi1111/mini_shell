/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:09:53 by tkashi            #+#    #+#             */
/*   Updated: 2024/03/31 18:01:08 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/**
 * Copies a substring from a given string.
 *
 * @param s The string from which to extract the substring.
 * @param start The starting index of the substring in the string.
 * @param len The maximum length of the substring.
 * @return The extracted substring, or NULL if memory allocation fails or if s is NULL.
 */
char *ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t res_len;
	char *sub;
	size_t s_len;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start > s_len)
	{
		res_len = 0;
		start = s_len;
	}
	else
	{
		res_len = s_len - start;
		if (len < res_len)
			res_len = len;
	}
	sub = (char *)malloc((res_len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	ft_strlcpy(sub, s + start, res_len + 1);
	return (sub);
}
