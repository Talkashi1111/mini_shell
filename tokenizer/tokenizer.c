/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:18:41 by achappui          #+#    #+#             */
/*   Updated: 2024/03/20 14:18:41 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*to_end_of_quote(char *ptr, char quote_type);
void	to_token_start(char **start);
char	get_token_type(char *str);
void	to_operator_end(char **end);
void	to_word_end(char **end);
char	*ft_substr(char const *s, unsigned int start, size_t len);

void	free_token_list(t_token_list *begin)
{
	t_token_list	*next;

	while (begin)
	{
		next = begin->next;
		free(begin->str);
		free(begin);
		begin = next;
	}
}

t_token_list	*new_token(char *str, char type)
{
	t_token_list	*new_token;

	new_token = (t_token_list *)malloc(sizeof(t_token_list));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->str = str;
	new_token->next = NULL;
	return (new_token);
}

void	add_back_token(t_token_list **begin, t_token_list *token)
{
	t_token_list	*index;

	if (*begin == NULL)
		*begin = token;
	else
	{
		index = *begin;
		while (index->next)
			index = index->next;
		index->next = token;
	}
}

void	*tokenizer_error(t_token_list *begin, char *str, char err_no)
{
	if (err_no == 2)
		free(str);
	free_token_list(begin);
	return (NULL);
}

/*
This function do a first separation of the input. Meaning the different elements of the input are identified
such as words and operators by the delimiters, without getting rid of the quotes or the dollars. into tokens and check if arrangement is correct (operators)
delimiters:  < << >> > | && || WHITE_SPACES
Arguments: input
Return: token list / NULL if failed
*/
t_token_list	*tokenizer(char *str)
{
	char 			*start;
	char			*end;
	char			type;
	t_token_list 	*begin;
	t_token_list	*token;

	begin = NULL;
	start = str;
	end = str;
	while (1)
	{
		to_token_start(&start);
		if (*start == '\0')
			break ;
		end = start;
		type = get_token_type(start);
		if (type == WORD)
			to_word_end(&end);
		else
			to_operator_end(&end);
		str = ft_substr(start, 0, end - start);
		if (!str)
			return(tokenizer_error(begin, str, 1));
		token = new_token(str, type);
		if (!token)
			return(tokenizer_error(begin, str, 2));
		add_back_token(&begin, token);
		if (*end == '\0')
			break ;
		start = end;
	}
	return (begin);
}
