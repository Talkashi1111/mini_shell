/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:25:33 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 14:52:31 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Locates the first occurrence of the character 'c'
 * (converted to a char) in the string 's'.
 *
 * @param s The string to search in.
 * @param c The character to search for. Also could be '\0'.
 * @return A pointer to the first occurrence of the character 'c',
 *  in the string 's',
 *         or NULL if the character is not found.
 */
char	*ft_strchr(const char *s, int c)
{
	char	*ptr;
	char	ch;

	ch = (char)c;
	ptr = (char *)s;
	while (*ptr != '\0')
	{
		if (*ptr == ch)
			return (ptr);
		ptr++;
	}
	if (ch == '\0')
		return (ptr);
	return (NULL);
}
