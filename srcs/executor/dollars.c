/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:43:53 by achappui          #+#    #+#             */
/*   Updated: 2024/04/07 20:15:59 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

void	fill_with_last_exit_status(int n, char **new_str)
{
	unsigned int	u_n;
	short			i;
	char			tab[10];

	i = 0;
	if (n < 0)
	{
		*(*new_str)++ = '-';
		u_n = -n;
	}
	else
		u_n = n;
	while (u_n > 0)
	{
		tab[i++] = u_n % 10 + '0';
		u_n /= 10;
	}
	while (--i >= 0)
		*(*new_str)++ = tab[i];
}

void	fill_from_dollar(char **old_str, char **new_str, t_minishell *info)
{
	char			*varvalue;
	unsigned int	varname_len;

	(*old_str)++;
	if (**old_str == '?')
	{
		fill_with_last_exit_status(info->last_exit_status, new_str);
		(*old_str)++;
	}
	else
	{
		varname_len = get_varname_len(*old_str);
		varvalue = find_envp_arg(info->envp, *old_str, varname_len);
		while (*varvalue != '\0')
			*(*new_str)++ = *varvalue++;
		old_str += varname_len;
	}
}

void	fill_new_str(char *old_str, char *new_str, t_minishell *info)
{
	unsigned int	tmp;

	while (*old_str != '\0')
	{
		if (*old_str == '\'')
		{
			tmp = (to_end_of_quote(old_str) - old_str) + 1;
			while (tmp--)
				*new_str++ = *old_str++;
		}
		else if (*old_str == '$')
			fill_from_dollar(&old_str, &new_str, info);
		else
			*new_str++ = *old_str++;
	}
}

char	expand_dollars(t_token_list *args, t_minishell *info)
{
	unsigned int	len;
	char			*new_str;

	while (args)
	{
		len = count_final_length(args->str, info);
		new_str = (char *)malloc((len + 1) * sizeof(char));
		if (!new_str)
			return (MALLOC_ERROR);
		new_str[len] = '\0';
		fill_new_str(args->str, new_str, info);
		free(args->str);
		args->str = new_str;
		args = args->next;
	}
	return (OK);
}