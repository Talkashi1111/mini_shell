/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:18:41 by achappui          #+#    #+#             */
/*   Updated: 2024/04/01 23:36:59 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

void	free_token_list(t_token_list *node)
{
	t_token_list	*next;

	while (node)
	{
		next = node->next;
		node->next = NULL;
		free(node->str);
		free(node);
		node = next;
	}
}

t_token_list	*new_token(char *str, char type)
{
	t_token_list	*token;
	
	token = (t_token_list *)ft_calloc(1, sizeof(t_token_list));
	if (!token)
		return (NULL);
	token->str = ft_strdup(str);
	if (!token->str)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	return (token);
}

t_token_list	*create_token(void)
{
	t_token_list	*token;

	token = (t_token_list *)ft_calloc(1, sizeof(t_token_list));
	return (token);
}

t_token_list	*copy_token(t_token_list *token)
{
	t_token_list	*new_node;

	new_node = create_token();
	if (!new_node)
		return (NULL);
	new_node->str = ft_strdup(token->str);
	if (!new_node->str)
	{
		free(new_node);
		return (NULL);
	}
	new_node->type = token->type;
	return (new_node);
}

/**
 * This function performs the initial separation of the input,
 * identifying different elements such as words and operators using delimiters,
 * without removing quotes or dollars.
 * It converts the input into tokens and checks if the arrangement of operators is correct.
 * 
 * Delimiters: < << >> > | && || WHITE_SPACES
 * 
 * @param str The input string to be tokenized
 * @return A pointer to the token list if successful, NULL if failed
 */
t_token_list	*tokenizer(char *str)
{
	char			*end;
	t_token_list 	*head;
	t_token_list	*tail;
	t_token_list	*token;

	head = NULL;
	while (str && *str != '\0')
	{
		skip_whitespace_start(&str);
		if (*str == '\0')
			break ;
		end = str;
		token = create_token();
		if (!token)
		{
			free_token_list(head);
			return (NULL);
		}
		token->type = get_token_type(str);
		if (token->type == WORD)
			to_word_end(&end);
		else
			to_operator_end(&end);
		token->str = ft_substr(str, 0, end - str);
		if (!token->str)
		{
			free_token_list(head);
			free(token);
			return (NULL);
		}
		if (!head)
		{
			head = token;
			tail = head;
		}
		else
		{
			tail->next = token;
			tail = token;
		}
		str = end;
	}
	return (head);
}
