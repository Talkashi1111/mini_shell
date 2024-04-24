/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:43:53 by achappui          #+#    #+#             */
/*   Updated: 2024/04/23 23:56:06 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	search_var_len_dollar(t_minishell *info, char **str,
	t_dollar_info *dollar_info, long long *i)
{
	unsigned int	varname_len;
	char			*delimiters;

	delimiters = " \t\n\v\f\r'*\"$=\0";
	varname_len = 0;
	while (ft_strchr(delimiters, (*str + 1)[varname_len]) == 0)
		varname_len++;
	(dollar_info->seq) = find_envp_arg(info->envp, *str + 1, varname_len);
	if ((dollar_info->seq))
		*i = ft_strlen((dollar_info->seq));
	*str += varname_len + 1;
}

long long	with_dollar(char **str, t_dollar_info *dollar_info,
	unsigned int *len, t_minishell *info)
{
	long long		i;

	i = 0;
	if ((*str)[i + 1] == '?')
	{
		(dollar_info->seq) = ft_itoa(info->last_exit_status);
		if (!(dollar_info->seq))
			return (-1);
		dollar_info->to_free = TRUE;
		i = ft_strlen((dollar_info->seq));
		*str += 2;
	}
	else
		search_var_len_dollar(info, str, dollar_info, &i);
	*len += i;
	return (i);
}

long long	no_dollar(char **str, char **seq, unsigned int *len)
{
	long long	i;

	*seq = *str;
	i = 0;
	while ((*str)[i] != '$' && (*str)[i] != '\0')
	{
		if ((*str)[i] == '\'')
			i += to_end_of_quote(*str + i) - (*str + i);
		i++;
	}
	*str += i;
	*len += i;
	return (i);
}

int	assign_null_var(t_minishell *info, unsigned int len, char **new_str, t_dollar_info *dollar_info)
{
		*new_str = (char *)ft_calloc((len + 1), sizeof(char));

		if (!(*new_str))
		{
			info->last_exit_status = errno;
			if (dollar_info->to_free == TRUE)
			{
				free(dollar_info->seq);
				dollar_info->seq = NULL;
			}
			return (MALLOC_ERROR);
		}
		(*new_str)[len] = '\0';
		return (OK);

}

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
/* 		new_str = (char *)malloc((len + 1) * sizeof(char));
		new_str[len] = '\0';
		if (!new_str)
		{
			if (dollar_info.to_free == TRUE)
				free(dollar_info.seq);
			return (NULL);
		} */
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
		if (!(ft_strchr(tmp_ptr->next->str, '$') && tmp_ptr->next->str[0] != '\0' && tmp_ptr->next->str[1] != '\0'))
		{
			tmp_ptr = tmp_ptr->next;
			continue ;
		}
		to_free = (t_token_list *)(tmp_ptr->next->str);
		tmp_ptr->next->str = expand_dollar(tmp_ptr->next->str, 0, info);
		free(to_free);
		if (!tmp_ptr->next->str)
		{
			info->last_exit_status = errno;
			*args = (*args)->next;
			return (info->last_exit_status);
		}
		else if (tmp_ptr->next->str[0] == '\0')
		{
			to_free = tmp_ptr->next;
			tmp_ptr->next = tmp_ptr->next->next;
			free(to_free->str);
			free(to_free);
		}
		else if (runtime && tmp_ptr->next->type == STDIN_HEREDOC)
			tmp_ptr = tmp_ptr->next->next;
		else
			tmp_ptr = tmp_ptr->next;
	}
	*args = (*args)->next;
	return (OK);
}
