/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:46:44 by tkashi            #+#    #+#             */
/*   Updated: 2023/11/01 18:46:47 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdlib.h>

static t_libft	*ft_apply_lst(t_libft *lst,
	t_libft *new_lst,
	void *(*f)(void *),
	void (*del)(void *))
{
	void	*new_content;
	t_libft	*curr;

	curr = new_lst;
	while (lst)
	{
		new_content = f(lst->content);
		if (!new_content)
		{
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		curr->next = ft_lstnew(new_content);
		if (curr->next == NULL)
		{
			del(new_content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		curr = curr->next;
		lst = lst->next;
	}
	return (new_lst);
}

t_libft	*ft_lstmap(t_libft *lst, void *(*f)(void *), void (*del)(void *))
{
	t_libft	*new_lst;
	t_libft	*node;

	new_lst = ft_lstnew(NULL);
	if (!new_lst)
		return (NULL);
	node = ft_apply_lst(lst, new_lst, f, del);
	if (!node)
		return (NULL);
	node = new_lst;
	new_lst = new_lst->next;
	free(node);
	return (new_lst);
}
