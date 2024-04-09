/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:43:53 by achappui          #+#    #+#             */
/*   Updated: 2024/04/09 19:37:24 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "minishell.h"

long long	with_dollar(char **str, char **seq, unsigned int *len, bool *to_free, t_minishell *info)
{
	char			*delimiters;
	unsigned int	varname_len;
	long long		i;
	
	i = 0;
	delimiters = " \t\n\v\f\r'*\"$\0";
	if ((*str)[i + 1] == '?')
	{
		*seq = ft_itoa(info->last_exit_status);
		if (!*seq)
			return (-3);
		*to_free = TRUE;
		i = ft_strlen(*seq);
		*str += 1 + 1;
	}
	else
	{
		varname_len = 0;
		while (ft_strchr(delimiters, (*str + 1)[varname_len]) == NULL)
			varname_len++;
		*seq = find_envp_arg(info->envp, *str + 1 , varname_len);
		if (*seq)
			i = ft_strlen(*seq);
		*str += varname_len + 1;
	}
	*len += i;
	return (i);
}

long long no_dollar(char **str, char **seq, unsigned int *len)
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


char	*expand_dollar(char *str, unsigned int len, t_minishell *info)
{
	char		*new_str;
	char		*seq;
	bool		to_free;
	long long	i;

	to_free = FALSE;
	new_str = NULL;
	if (str[0] == '$')
		i = with_dollar(&str, &seq, &len, &to_free, info);
	else
		i = no_dollar(&str, &seq, &len);
	if (i == -3)
		return (NULL);
	if (*str == '\0')
	{
		new_str = (char *)malloc((len + 1) * sizeof(char));
		new_str[len] = '\0';
		if (!new_str)
			return (NULL);
	}
	else
		new_str = expand_dollar(str, len, info);
	while (i)
		new_str[--len] = seq[--i];
	if (to_free)
		free(seq);
	return (new_str);
}






char	expand_dollars(t_token_list *args, t_minishell *info)
{
	while (args)
	{
		args->str = expand_dollar(args->str, 0, info);
		if (!args->str)
			return (MALLOC_ERROR);
		args = args->next;
	}
	return (OK);
}



















// void	fill_with_last_exit_status(int n, char **new_str)
// {
// 	unsigned int	u_n;
// 	short			i;
// 	char			tab[10];

// 	i = 0;
// 	if (n < 0)
// 	{
// 		*(*new_str)++ = '-';
// 		u_n = -n;
// 	}
// 	else
// 		u_n = n;
// 	while (u_n > 0)
// 	{
// 		tab[i++] = u_n % 10 + '0';
// 		u_n /= 10;
// 	}
// 	while (--i >= 0)
// 		*(*new_str)++ = tab[i];
// }

// void	fill_from_dollar(char **old_str, char **new_str, t_minishell *info)
// {
// 	char			*varvalue;
// 	unsigned int	varname_len;

// 	(*old_str)++;
// 	if (**old_str == '?')
// 	{
// 		fill_with_last_exit_status(info->last_exit_status, new_str);
// 		(*old_str)++;
// 	}
// 	else
// 	{
// 		varname_len = get_varname_len(*old_str);
// 		varvalue = find_envp_arg(info->envp, *old_str, varname_len);
// 		while (*varvalue != '\0')
// 			*(*new_str)++ = *varvalue++;
// 		old_str += varname_len;
// 	}
// }

// void	fill_new_str(char *old_str, char *new_str, t_minishell *info)
// {
// 	unsigned int	tmp;

// 	while (*old_str != '\0')
// 	{
// 		if (*old_str == '\'')
// 		{
// 			tmp = (to_end_of_quote(old_str) - old_str) + 1;
// 			while (tmp--)
// 				*new_str++ = *old_str++;
// 		}
// 		else if (*old_str == '$')
// 			fill_from_dollar(&old_str, &new_str, info);
// 		else
// 			*new_str++ = *old_str++;
// 	}
// }

// char	expand_dollars(t_token_list *args, t_minishell *info)
// {
// 	unsigned int	len;
// 	char			*new_str;

// 	while (args)
// 	{
// 		len = count_final_length(args->str, info);
// 		new_str = (char *)malloc((len + 1) * sizeof(char));
// 		if (!new_str)
// 			return (MALLOC_ERROR);
// 		new_str[len] = '\0';
// 		fill_new_str(args->str, new_str, info);
// 		free(args->str);
// 		args->str = new_str;
// 		args = args->next;
// 	}
// 	return (OK);
// }