/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 12:21:29 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 14:51:46 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Searches for the first occurrence of a substring within a string,
 * limited to a certain length.
 *
 * @param haystack The string to search within.
 * @param needle The substring to search for.
 * @param len The maximum number of characters to search within the haystack.
 * @return A pointer to the first occurrence of the substring
 * within the haystack, or NULL if not found.
 */
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	needle_len;
	size_t	haystack_len;

	if (*needle == '\0')
		return ((char *)haystack);
	if (haystack == needle)
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	haystack_len = ft_strlen(haystack);
	if (len > haystack_len)
		len = haystack_len;
	i = 0;
	while (i + needle_len <= len)
	{
		if (haystack[i] == needle[0])
		{
			if (ft_strncmp(&haystack[i], needle, needle_len) == 0)
				return ((char *)&haystack[i]);
		}
		i++;
	}
	return (NULL);
}
