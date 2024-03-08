/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 21:28:12 by tkashi            #+#    #+#             */
/*   Updated: 2023/10/26 21:13:12 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

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
