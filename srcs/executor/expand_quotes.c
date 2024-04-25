/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 20:44:08 by achappui          #+#    #+#             */
/*   Updated: 2024/04/25 19:17:44 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned int	without_quotes_len(char *str)
{
	unsigned int	tmp;
	unsigned int	final_len;

	final_len = 0;
	while (*str)
	{
		if (*str == '\'' || *str == '"')
		{
			tmp = to_end_of_quote(str) - str;
			if (tmp)
			{
				final_len += tmp + 1 - 2;
				*str = '\0';
				*(str + tmp) = '\0';
				str += tmp + 1;
				continue ;
			}
		}
		str++;
		final_len++;
	}
	return (final_len);
}

void	fill_without_quotes(char *start_ptr, char *end_ptr, char *new_str)
{
	while (start_ptr != end_ptr)
	{
		if (*start_ptr != '\0')
			*new_str++ = *start_ptr;
		start_ptr++;
	}
}

int	remove_quotes(t_token_list *args, t_minishell *info, char runtime)
{
	char			*end_ptr;
	unsigned int	len;
	char			*new_str;

	while (args)
	{
		end_ptr = args->str;
		while (*end_ptr)
			end_ptr++;
		len = without_quotes_len(args->str);
		new_str = (char *)malloc((len + 1) * sizeof(char));
		if (!new_str)
		{
			info->last_exit_status = errno;
			return (info->last_exit_status);
		}
		new_str[len] = '\0';
		fill_without_quotes(args->str, end_ptr, new_str);
		free(args->str);
		args->str = new_str;
		if (runtime && args->type == STDIN_HEREDOC)
			args = args->next;
		args = args->next;
	}
	return (OK);
}
