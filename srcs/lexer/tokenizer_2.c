/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 13:36:32 by achappui          #+#    #+#             */
/*   Updated: 2024/04/17 22:52:36 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*to_end_of_quote(char *str)
{
	char	quote_type;
	char	*saved_str;

	quote_type = *str;
	saved_str = str;
	str++;
	while (*str != '\0')
	{
		if (*str == quote_type)
			return (str);
		str++;
	}
	return (saved_str);
}

void	skip_whitespace_start(char **start)
{
	while (ft_isspace(**start))
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
	else if (*str == '(')
		return (OPENPAR);
	else if (*str == ')')
		return (CLOSEPAR);
	else
		return (WORD);
}

void	to_operator_end(char **end)
{
	if (**end != '(' && **end != ')' && *((*end) + 1) == **end)
		(*end)++;
	(*end)++;
}

void	to_word_end(char **end)
{
	while (**end != '\0')
	{
		if (**end == '"' || **end == '\'')
			*end = to_end_of_quote(*end);
		else if (ft_isspace(**end))
			break ;
		else if (**end == '&' && *((*end) + 1) == '&') 
			break ;
		else if (**end == '|')
			break ;
		else if (**end == '>')
			break ;
		else if (**end == '<')
			break ;
		else if (**end == '(')
			break ;
		else if (**end == ')')
			break ;
		(*end)++;
	}
}
