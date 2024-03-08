/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:48:38 by tkashi            #+#    #+#             */
/*   Updated: 2023/10/30 17:48:41 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	is_nul_term(const char *str, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (str[i] == '\0')
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	src_len;
	size_t	dest_len;
	size_t	i;
	size_t	j;

	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	if (!is_nul_term(dst, size))
		return (src_len + size);
	dest_len = ft_strlen(dst);
	i = 0;
	j = dest_len;
	while (src[i] != '\0' && j < size - 1)
		dst[j++] = src[i++];
	dst[j] = '\0';
	return (src_len + dest_len);
}
