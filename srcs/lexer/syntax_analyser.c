/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:52:46 by achappui          #+#    #+#             */
/*   Updated: 2024/03/22 15:52:46 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

char	syntax_error(char *str)
{
	ft_fprintf(STDERR_FILENO, "shell: syntax error near '%s'\n", str);
	return (SYNTAX_ERROR);
}

char	analyse_middle_token(t_token_list *token, unsigned int *openpar_count, unsigned int *closepar_count)
{
	if ((token->type == OR || token->type == AND || token->type == PIPE) &&
		(token->next->type == OR || token->next->type == AND || \
		token->next->type == PIPE || token->next->type == CLOSEPAR))
		return (SYNTAX_ERROR);
	else if ((token->type == STDIN || token->type == STDIN_HEREDOC || \
			token->type == STDOUT || token->type == STDOUT_APPEND) && \
			token->next->type != WORD)
		return (SYNTAX_ERROR);
	else if (token->type == OPENPAR)
	{
		(*openpar_count)++;
		if (token->next->type == AND || token->next->type == OR || \
		token->next->type == PIPE || token->next->type == CLOSEPAR)
			return (SYNTAX_ERROR);
	}
	else if (token->type == CLOSEPAR)
	{
		(*closepar_count)++;
		if (token->next->type == STDIN || \
		token->next->type == STDIN_HEREDOC || token->next->type == STDOUT || \
		token->next->type == STDOUT_APPEND || token->next->type == WORD)
			return (SYNTAX_ERROR);
	}
	return (OK);
}

/**
 * Checks the syntax of a token list.
 * on check l'ordre de gauche a droite donc on fait jamais de verif avec ce qu'il y a a gauche
 *
 * @param token The token list to analyze.
 * @return The result of the syntax analysis.
 *         - NOT_FOUND if the token list is NULL.
 *         - SYNTAX_ERROR if there is a syntax error in the token list.
 *         - OK if the syntax analysis is successful.
 */
char	syntax_analyser(t_token_list *token)
{
	unsigned int	openpar_count;
	unsigned int	closepar_count;

	openpar_count = 0;
	closepar_count = 0;
	if (token->type == OR || token->type == AND || token->type == PIPE || \
		token->type == CLOSEPAR)
		return (syntax_error(token->str));
	while (token->next)
	{
		if (analyse_middle_token(token, &openpar_count, &closepar_count) != OK)
			return (syntax_error(token->next->str));
		token = token->next;
	}
	if (token->type == OPENPAR)
		openpar_count++;
	if (token->type == CLOSEPAR)
		closepar_count++;
	if ((token->type == OR || token->type == AND || token->type == PIPE || \
		token->type == OPENPAR) || (openpar_count != closepar_count))
		return (syntax_error("\\n"));
	return (OK);
}