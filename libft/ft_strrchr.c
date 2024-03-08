/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:54:39 by tkashi            #+#    #+#             */
/*   Updated: 2023/10/25 18:08:04 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*temp;
	char	ch;

	ch = (char)c;
	temp = NULL;
	while (*s != '\0')
	{
		if (*s == ch)
			temp = (char *)s;
		s++;
	}
	if (ch == '\0')
		temp = (char *)s;
	return (temp);
}
