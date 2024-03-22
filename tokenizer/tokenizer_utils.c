/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 13:36:32 by achappui          #+#    #+#             */
/*   Updated: 2024/03/22 13:38:13 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*to_end_of_quote(char *ptr, char quote_type)
{
	char	*saved_ptr;

	saved_ptr = ptr; //car si il n'y a que une quote 
	while (*ptr != '\0')
	{
		if (*ptr == quote_type)
			return (ptr);
		ptr++;
	}
	return (saved_ptr);
}

void	to_token_start(char **start)
{
	while (**start == ' ' || **start == '\t' || \
			**start == '\n' || **start == '\v' || \
			**start == '\f' || **start == '\r')
		(*start)++;
}

char	get_token_type(char *str)
{
	if (*str == '&' && *(str + 1) == '&')
		return (AND);
	else if (*str == '|' && *(str + 1) == '|')
		return (OR);
	else if (*str == '>' && *(str + 1) == '>')
		return (STDOUT_APPEND);
	else if (*str == '<' && *(str + 1) == '<')
		return (STDIN_HEREDOC);
	else if (*str == '|')
		return (PIPE);
	else if (*str == '>')
		return (STDOUT);
	else if (*str == '<')
		return (STDIN);
	else
		return (WORD);
}

void	to_operator_end(char **end)
{
	if (*((*end) + 1) == **end)
		(*end)++;
	(*end)++;
}

void	to_word_end(char **end)
{
	while (**end != '\0')
	{
		if (**end == '"' || **end == '\'')
			*end = to_end_of_quote(*end, **end);
		else if (**end == ' ' || **end == '\t' || \
				**end == '\n' || **end == '\v' || \
				**end == '\f' || **end == '\r')
			break ;
		else if (**end == '&' && *((*end) + 1) == '&') //car on ne traite pas l'operateur &, ainsi il peut faire partie d'un mot ?
			break ;
		else if (**end == '|')
			break ;
		else if (**end == '>')
			break ;
		else if (**end == '<')
			break ;
		(*end)++;
	}
}
