/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 21:28:12 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 14:51:06 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/**
 * Concatenates two strings and returns a new string.
 *
 * @param s1 The first string to be concatenated.
 * @param s2 The second string to be concatenated.
 * @return The concatenated string, or NULL if memory allocation fails.
 */
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	size_t	size_total;

	size_total = ft_strlen(s1) + ft_strlen(s2) + 1;
	dest = (char *)malloc(size_total * sizeof(char));
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, s1, size_total);
	ft_strlcat(dest, s2, size_total);
	return (dest);
}
