/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 13:36:32 by achappui          #+#    #+#             */
/*   Updated: 2024/04/02 23:33:04 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*to_end_of_quote(char *ptr, char quote_type)
{
	char	*saved_ptr;

	saved_ptr = ptr;
	while (*ptr != '\0')
	{
		if (*ptr == quote_type)
			return (ptr);
		ptr++;
	}
	return (saved_ptr);
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
		{
			printf("NOUS SOMME LA \n");
			*end = to_end_of_quote(*end + 1, **end);
			printf("SYMBOLE: %c\n", **end);
		}
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

void	display_token_list(t_token_list *token)
{
	if (!token)
	{
		ft_printf("empty token list\n");
		return ;
	}
	while (token)
	{
		ft_printf("\"%s\" TYPE: %i ", token->str, token->type);
		token = token->next;
	}
	ft_printf("\n");
}
