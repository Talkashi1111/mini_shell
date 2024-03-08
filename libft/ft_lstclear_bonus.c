/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:38:03 by tkashi            #+#    #+#             */
/*   Updated: 2023/11/01 18:38:06 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

void	ft_lstclear(t_libft **lst, void (*del)(void *))
{
	t_libft	*curr;
	t_libft	*temp;

	curr = *lst;
	while (curr != NULL)
	{
		temp = curr;
		curr = curr->next;
		ft_lstdelone(temp, del);
	}
	*lst = NULL;
}
