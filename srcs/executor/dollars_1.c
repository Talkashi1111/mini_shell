/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:43:53 by achappui          #+#    #+#             */
/*   Updated: 2024/04/09 16:51:34 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// unsigned int	get_varname_len(char *str);

// char	is_in_strn(char c, char *str, unsigned int n)
// {
// 	while (n)
// 		if (str[--n] == c)
// 			return (1);
// 	return (0);
// }

// unsigned int	get_varname_len(char *str)
// {
// 	unsigned int	len;
// 	char		*delimiters;

// 	len = 0;
// 	delimiters = " \t\n\v\f\r'*\"$\0";
// 	while (TRUE)
// 	{
// 		if (is_in_strn(str[len], delimiters, 11))
// 			return (len);
// 		len++;
// 	}
// }

// unsigned int	int_len(int n)
// {
// 	unsigned int	len;

// 	if (n == 0)
// 		return (1);
// 	len = 0;
// 	if (n < 0)
// 		len++;
// 	while (n != 0)
// 	{
// 		n /= 10;
// 		len++;
// 	}
// 	return (len);
// }

// unsigned int	len_from_dollar(char **str, t_minishell *info)
// {
// 	unsigned int	len;
// 	unsigned int	varname_len;

// 	(*str)++;
// 	if (**str == '?')
// 	{
// 		(*str)++;
// 		len = int_len(info->last_exit_status);
// 	}
// 	else
// 	{
// 		varname_len = get_varname_len(*str);
// 		len = ft_strlen(find_envp_arg(info->envp, *str, varname_len));
// 		(*str) += varname_len;
// 	}
// 	return (len);
// }

// unsigned int	count_final_length(char *str, t_minishell *info)
// {
// 	unsigned int	tmp;
// 	unsigned int	final_length;

// 	final_length = 0;
// 	while (*str != '\0')
// 	{
// 		if (*str == '\'')
// 		{
// 			tmp = (to_end_of_quote(str) - str) + 1;
// 			str += tmp;
// 			final_length += tmp;
// 		}
// 		else if (*str == '$')
// 			final_length += len_from_dollar(&str, info);
// 		else
// 		{
// 			str++;
// 			final_length++;	
// 		}
// 	}
// 	return (final_length);
// }