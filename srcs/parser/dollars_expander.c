/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars_expander.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:43:53 by achappui          #+#    #+#             */
/*   Updated: 2024/04/06 20:28:34 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

unsigned int	skip_quotes(char **str)
{
	unsigned int	len;

	len = 1;
	(*str)++;
	while (**str != '\'')
	{
		(*str)++;
		len++;
	}
	return (len);
}



char	is_in_strn(char c, char *str, unsigned int n)
{
	while (n)
		if (str[--n] == c)
			return (1);
	return (0);
}

unsigned int	get_varname_len(char *str)
{
	unsigned int	len;
	char		*delimiters;

	len = 0;
	delimiters = " \t\n\v\f\r'*\"$\0";
	while (TRUE)
	{
		if (is_in_strn(str[len], delimiters, 11))
			return (len);
		len++;
	}
}

unsigned int	int_len(int n)
{
	unsigned int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

unsigned int	count_final_length(char *str, t_minishell *info)
{
	unsigned int	tmp;
	unsigned int	final_length;
	unsigned int	varname_len;

	final_length = 0;
	while (*str != '\0')
	{
		if (*str == '\'')
		{
			tmp = to_end_of_quote(str, '\'') - (str) + 1;
			final_length += tmp; //a verifier
			while (tmp--)
				str++;
		}
		else if (*str == '$')
		{
			str++;
			if (*str == '?') //a verifier
			{
				final_length += int_len(info->last_exit_status);
				str++;
			}
			else
			{
				varname_len = get_varname_len(str);
				final_length += ft_strlen(find_envp_arg(info->envp, str, varname_len));
				while (varname_len--)
					str++;
			}
		}
		else
		{
			str++;
			final_length++;	
		}
	}
	return (final_length);
}

void	handle_last_exit_status(int n, char **new_str)
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

void	fill_new_str(char *old_str, char *new_str, t_minishell *info)
{
	unsigned int	varname_len;
	unsigned int	tmp;
	char			*value;

	while (*old_str != '\0')
	{
		if (*old_str == '\'')
		{
			*new_str++ = *old_str++;
			tmp = to_end_of_quote(old_str - 1, '\'') - (old_str - 1);
			if (tmp)
				while (--tmp)
					*new_str++ = *old_str++;
		}
		else if (*old_str == '$')
		{
			old_str++;
			if (*old_str == '?')
			{
				handle_last_exit_status(info->last_exit_status, &new_str);
				old_str++;
			}
			else
			{
				varname_len = get_varname_len(old_str);
				value = find_envp_arg(info->envp, old_str + 1, varname_len);
				while (*value != '\0')
					*new_str++ = *value++;
				old_str += varname_len;
			}
		}
		else
			*new_str++ = *old_str++;
	}
}

void	ERROR_A_DEFINIR()
{
	ft_printf("MALLOC ERROR de ERROR_A_DEFINIR\n");
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
			return (MALLOC_ERROR); // ERROR_A_DEFINIR();
		new_str[len] = '\0';
		fill_new_str(args->str, new_str, info);
		free(args->str);
		args->str = new_str;
		args = args->next;
	}
	return (OK);
}


