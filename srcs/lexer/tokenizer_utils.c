/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:51:26 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/25 13:48:02 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token_list(t_token_list *token)
{
	t_token_list	*next;

	while (token)
	{
		next = token->next;
		token->next = NULL;
		free(token->str);
		free(token);
		token = next;
	}
}

t_token_list	*create_token(void)
{
	t_token_list	*token;

	token = (t_token_list *)ft_calloc(1, sizeof(t_token_list));
	return (token);
}

void	add_back_tokenizer(t_token_list *tl[3])
{
	if (!tl[HEAD])
	{
		tl[HEAD] = tl[TOKEN];
		tl[TAIL] = tl[HEAD];
	}
	else
	{
		tl[TAIL]->next = tl[TOKEN];
		tl[TAIL] = tl[TOKEN];
	}
}
