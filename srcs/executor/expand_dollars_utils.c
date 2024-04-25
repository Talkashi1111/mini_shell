/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:25:52 by achappui          #+#    #+#             */
/*   Updated: 2024/04/25 21:02:34 by achappui         ###   ########.fr       */
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

int	assign_null_var(t_minishell *info, unsigned int len,
	char **new_str, t_dollar_info *dollar_info)
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
