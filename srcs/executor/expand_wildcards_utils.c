/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 22:12:34 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 13:26:22 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_list	*last_list_elem(t_token_list *list)
{
	if (list)
		while (list->next)
			list = list->next;
	return (list);
}

int	assign_to_list_wildcard(t_minishell *info, t_token_list *tmp_ptr)
{
	t_token_list	*last;
	t_token_list	*wildcard_list;

	wildcard_list = NULL;
	if (get_wildcard(&wildcard_list, tmp_ptr->next->str, info) != OK)
		return (info->last_exit_status);
	last = last_list_elem(wildcard_list);
	last->next = tmp_ptr->next->next;
	free(tmp_ptr->next->str);
	free(tmp_ptr->next);
	tmp_ptr->next = wildcard_list;
	tmp_ptr = last;
	return (OK);
}

int	no_path_found_and_closedir(t_minishell *info, t_token_list **wildcard_list,
	char *pattern, DIR *dir)
{
	if ((*wildcard_list) == NULL)
	{
		(*wildcard_list) = new_token(pattern, WORD);
		if (!(*wildcard_list))
		{
			info->last_exit_status = errno;
			free_token_list((*wildcard_list));
			return (info->last_exit_status);
		}
	}
	if (closedir(dir) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "closedir: %s\n", strerror(errno));
	}
	return (OK);
}
