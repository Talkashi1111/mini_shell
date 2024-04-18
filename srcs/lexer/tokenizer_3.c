/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 18:42:12 by achappui          #+#    #+#             */
/*   Updated: 2024/04/06 18:42:12 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token_list(t_token_list *token, char redi, t_minishell *info)
{
	t_token_list	*next;

	while (token)
	{
		if (redi && token->type == STDIN_HEREDOC && unlink(token->next->str) == -1)
		{
			printf("%s\n", token->next->str);
			info->last_exit_status = errno;
			ft_fprintf(STDERR_FILENO, "unlink: %s\n", strerror(errno));
		}
		next = token->next;
		token->next = NULL;
		free(token->str);
		free(token);
		token = next;
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

t_token_list	*copy_token(t_token_list *token, t_minishell *info)
{
	t_token_list	*new_node;

	new_node = create_token();
	if (!new_node)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "bash: malloc: %s\n", strerror(errno));
		return (NULL);
	}
	new_node->str = ft_strdup(token->str);
	if (!new_node->str)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "bash: malloc: %s\n", strerror(errno));
		free(new_node);
		return (NULL);
	}
	new_node->type = token->type;
	return (new_node);
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