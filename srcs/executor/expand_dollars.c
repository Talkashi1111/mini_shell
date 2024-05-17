/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:43:53 by achappui          #+#    #+#             */
/*   Updated: 2024/05/16 17:38:02 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_dollar(char *str, unsigned int len, t_minishell *info)
{
	char			*new_str;
	t_dollar_info	dollar_info;
	long long		i;

	dollar_info.to_free = FALSE;
	new_str = NULL;
	if (str[0] == '$')
		i = with_dollar(&str, &dollar_info, &len, info);
	else
		i = no_dollar(&str, &(dollar_info.seq), &len);
	if (i == -1)
		return (NULL);
	if (*str == '\0')
	{
		if (assign_null_var(info, len, &new_str, &dollar_info) != OK)
			return (NULL);
	}
	else
		new_str = expand_dollar(str, len, info);
	if (new_str)
		while (i)
			new_str[--len] = dollar_info.seq[--i];
	if (dollar_info.to_free == TRUE)
		free(dollar_info.seq);
	return (new_str);
}

void	remove_this_node(t_token_list *tmp_ptr)
{
	t_token_list	*to_free;

	to_free = tmp_ptr->next;
	tmp_ptr->next = tmp_ptr->next->next;
	free(to_free->str);
	free(to_free);
}

int	ft_is_skipable(t_token_list **tmp_ptr)
{
	if (ft_strchr((*tmp_ptr)->next->str, '$') == NULL || \
		(*tmp_ptr)->next->str[0] == '\0' || \
		(*tmp_ptr)->next->str[1] == '\0' || \
		ft_strncmp((*tmp_ptr)->next->str, "\"$\"", 4) == 0)
	{
		(*tmp_ptr) = (*tmp_ptr)->next;
		return (1);
	}
	return (0);
}

int	dollar_expansion_error(t_token_list **args, t_minishell *info)
{
	info->last_exit_status = errno;
	*args = (*args)->next;
	return (info->last_exit_status);
}

char	expand_dollars(t_token_list **args, t_minishell *info, char runtime)
{
	t_token_list	tmp;
	t_token_list	*tmp_ptr;
	t_token_list	*to_free;

	tmp_ptr = &tmp;
	tmp_ptr->next = *args;
	*args = tmp_ptr;
	while (tmp_ptr->next)
	{
		if (ft_is_skipable(&tmp_ptr))
			continue ;
		to_free = (t_token_list *)(tmp_ptr->next->str);
		tmp_ptr->next->str = expand_dollar(tmp_ptr->next->str, 0, info);
		free(to_free);
		if (!tmp_ptr->next->str)
			return (dollar_expansion_error(args, info));
		else if (tmp_ptr->next->str[0] == '\0')
			remove_this_node(tmp_ptr);
		else if (runtime && tmp_ptr->next->type == STDIN_HEREDOC)
			tmp_ptr = tmp_ptr->next->next;
		else
			tmp_ptr = tmp_ptr->next;
	}
	*args = (*args)->next;
	return (OK);
}
