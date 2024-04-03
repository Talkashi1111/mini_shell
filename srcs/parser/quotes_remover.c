/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_remover.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 20:44:08 by achappui          #+#    #+#             */
/*   Updated: 2024/04/02 23:42:47 by achappui         ###   ########.fr       */
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
			tmp = to_end_of_quote(str + 1, *str) - str;
			// printf("LEN2: %u\n", tmp);
			if (tmp)
			{
				final_len += tmp + 1 - 2;
				// printf("FINALLEN: %u\n", final_len);
				*str = '\0';
				*(str + tmp) = '\0';
				str += tmp + 1;
			}
		}
		else
		{
			str++;
			final_len++;
		}
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

char	remove_quotes(t_token_list *args)
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
		// printf("LEN %d\n", len);
		new_str = (char *)malloc((len + 1) * sizeof(char));
		if (!new_str)
			return (2);
		new_str[len] = '\0';
		fill_without_quotes(args->str, end_ptr, new_str);
		free(args->str);
		args->str = new_str;
		// printf("\"%s\"\n", args->str);
		args = args->next;
	}
	return (1);
}