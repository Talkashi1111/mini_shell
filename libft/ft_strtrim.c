/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 14:30:18 by tkashi            #+#    #+#             */
/*   Updated: 2023/10/30 18:20:45 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "libft.h"

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
