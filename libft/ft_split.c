/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 11:25:56 by tkashi            #+#    #+#             */
/*   Updated: 2024/03/08 23:00:43 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int	ft_calc_size(char const *s, char c)
{
	int	prev;
	int	i;
	int	count;

	prev = -1;
	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			if (i - prev - 1 > 0)
				count++;
			prev = i;
		}
		i++;
	}
	if (i - prev - 1 > 0)
		count++;
	return (count);
}

int	ft_add_splitted(char const *s, int start, int end, char **ret)
{
	*ret = ft_substr(s, start + 1, end - start - 1);
	if (*ret == NULL)
		return (0);
	return (1);
}

int	ft_apply_split(char const *s, char c, char **ret)
{
	int	prev;
	int	i;

	prev = -1;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			if (i - prev - 1 > 0)
			{
				if (!ft_add_splitted(s, prev, i, ret))
					return (0);
				ret++;
			}
			prev = i;
		}
		i++;
	}
	if (i - prev - 1 > 0)
	{
		if (!ft_add_splitted(s, prev, i, ret))
			return (0);
	}
	return (1);
}

/**
 * Splits a string into an array of substrings based on
 * a delimiter character.
 *
 * @param s The string to be split.
 * @param c The delimiter character.
 * @return An array of substrings if successful, NULL otherwise.
 * If the allocation fails, the function returns NULL.
 * If s is an empty string, the function returns a pointer to a
 * 2D array containing a single NULL.
 */
char	**ft_split(char const *s, char c)
{
	char	**ret;
	size_t	size;
	int		i;

	size = ft_calc_size(s, c);
	ret = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	if (!ft_apply_split(s, c, ret))
	{
		i = 0;
		while (ret[i] != NULL)
		{
			free(ret[i]);
			i++;
		}
		free(ret);
		return (NULL);
	}
	return (ret);
}
