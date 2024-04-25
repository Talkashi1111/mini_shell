/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:56:07 by achappui          #+#    #+#             */
/*   Updated: 2024/04/25 13:59:43 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_free(char *line)
{
	free(line);
	return (1);
}

void	fill_tab(char *tab, unsigned int size, uintptr_t n)
{
	char			*heredoc_path;
	unsigned int	i;

	heredoc_path = HEREDOC_PATH;
	i = -1;
	while (heredoc_path[++i])
		tab[i] = heredoc_path[i];
	tab[i++] = '/';
	tab[i] = '.';
	tab[size] = '\0';
	while (tab[--size] != '.')
	{
		tab[size] = n % 10 + 48;
		n /= 10;
	}
}

char	*ft_itoa_heredoc(uintptr_t n)
{
	uintptr_t		tab;
	unsigned int	size;

	tab = n;
	size = 1;
	while (tab >= 10 && size++)
		tab /= 10;
	size += ft_strlen(HEREDOC_PATH) + 2;
	tab = (uintptr_t)calloc(size + 1, sizeof(char));
	if (!tab)
		return (NULL);
	fill_tab((char *)tab, size, n);
	return ((char *)tab);
}
