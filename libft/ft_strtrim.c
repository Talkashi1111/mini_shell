/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 14:30:18 by tkashi            #+#    #+#             */
/*   Updated: 2024/03/08 23:00:13 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Removes leading and trailing characters specified in
 * the 'set' parameter from the string 's1'.
 *
 * @param s1 The string to be trimmed.
 * @param set The set of characters to be removed.
 * @return A new string with leading and trailing characters removed,
 * or NULL if memory allocation fails.
 */
char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	start;
	size_t	end;

	if (*s1 == '\0' || *set == '\0')
		return (ft_strdup(s1));
	end = ft_strlen(s1) - 1;
	start = 0;
	i = 0;
	while (i <= end && ft_strchr(set, s1[i]) != NULL)
	{
		start++;
		i++;
	}
	i = end;
	while (i > start && ft_strchr(set, s1[i]) != NULL)
	{
		end--;
		i--;
	}
	return (ft_substr(s1, start, end - start + 1));
}
