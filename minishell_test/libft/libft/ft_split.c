/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:27:03 by achappui          #+#    #+#             */
/*   Updated: 2024/01/17 16:11:09 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	ft_in_set(char const *set, char c)
{
	while (*set != '\0')
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static size_t	ft_split_size(char const *s, const char *set)
{
	size_t	size;

	size = 0;
	while (*s != '\0')
	{
		if (!ft_in_set(set, *s) && (*(s + 1) == '\0' || \
			ft_in_set(set, *(s + 1))))
			size++;
		s++;
	}
	return (size);
}

static char	**ft_split_free(char **tab)
{
	unsigned int	i;

	i = 0;
	while (tab[i] != NULL)
		free(tab[i++]);
	free(tab);
	return (NULL);
}

char	**ft_split(char const *s, const char *set)
{
	char			**tab;
	unsigned int	i;
	size_t			len;

	tab = (char **)malloc((ft_split_size(s, set) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (*s != '\0')
	{
		while (ft_in_set(set, *s))
			s++;
		len = 0;
		while (s[len] != '\0' && !ft_in_set(set, s[len]))
			len++;
		if (len)
		{
			tab[i] = ft_substr(s, 0, len);
			if (!tab[i++])
				return (ft_split_free(tab));
		}
		s += len;
	}
	tab[i] = NULL;
	return (tab);
}
