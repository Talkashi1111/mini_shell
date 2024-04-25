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

#include "minishell.h"

char	syntax_error(char *str, t_minishell *info)
{
	info->last_exit_status = SYNTAX_ERROR;
	ft_fprintf(info->saved_streams[1], "bash: syntax error near '%s'\n", str);
	return (info->last_exit_status);
}

char	analyse_middle_token(t_token_list *token,
	unsigned int *openpar_count, unsigned int *closepar_count)
{
	if (token->type >= OR && token->type <= PIPE && \
		token->next->type >= OR && token->next->type <= CLOSEPAR)
		return (SYNTAX_ERROR);
	else if (token->type >= STDIN && token->type <= STDOUT_APPEND && \
			token->next->type != WORD)
		return (SYNTAX_ERROR);
	else if (token->type == OPENPAR && ((*openpar_count)++ || 1) && \
			token->next->type >= OR && token->next->type <= CLOSEPAR)
		return (SYNTAX_ERROR);
	else if ((token->type == CLOSEPAR && ((*closepar_count)++ || 1) && \
			(token->next->type >= WORD && token->next->type <= STDOUT_APPEND)) \
			|| token->next->type == OPENPAR)
		return (SYNTAX_ERROR);
	else if (token->type == WORD && token->next->type == OPENPAR)
		return (SYNTAX_ERROR);
	return (OK);
}

/**
 * Checks the syntax of a token list.
 * on check l'ordre de gauche a droite donc on
 * fait jamais de verif avec ce qu'il y a a gauche
 * @param token The token list to analyze.
 * @return The result of the syntax analysis.
 *         - NOT_FOUND if the token list is NULL.
 *         - SYNTAX_ERROR if there is a syntax error in the token list.
 *         - OK if the syntax analysis is successful.
 */
char	syntax_analyser(t_token_list *token, t_minishell *info)
{
	unsigned int	openpar_count;
	unsigned int	closepar_count;

	openpar_count = 0;
	closepar_count = 0;
	if (token->type >= OR && token->type <= CLOSEPAR)
		return (syntax_error(token->str, info));
	while (token->next)
	{
		if (analyse_middle_token(token, &openpar_count, &closepar_count) != OK)
			return (syntax_error(token->next->str, info));
		token = token->next;
	}
	if (token->type == OPENPAR)
		openpar_count++;
	if (token->type == CLOSEPAR)
		closepar_count++;
	if ((token->type == OR || token->type == AND || token->type == PIPE || \
		token->type == OPENPAR) || (openpar_count != closepar_count))
		return (syntax_error("\\n", info));
	return (OK);
}
