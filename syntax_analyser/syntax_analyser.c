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

#include "parsing.h"


/*
Cette fonction parcour la list chainee de token
verifiant si l'ordre des token est valide
retourn 1 if syntax error
retourn 0 if no error
*/

char	syntax_analyser(t_token_list *token) //est-ce que token est forcement non NULL, donc qu'il y a qqch dedans ? (si ca avait foirer le process est anuler mais si c'est vide alors token list est null)
{
	if (token->type >= OR)
	{
		printf("bash: syntax error near '%s'\n", token->str); //mettre mon printf
		return (1);
	}
	while (token->next)
	{
		if ((token->type >= OR) && token->next->type >= OR)
		{
			printf("bash: syntax error near '%s'\n", token->next->str); //mettre mon printf
			return (1);
		}
		else if ((token->type >= STDIN && token->type <= STDOUT_APPEND)  && token->next->type != WORD)
		{
			printf("bash: syntax error near '%s'\n", token->next->str); //mettre mon printf
			return (1);
		}
		token = token->next
	}
	if (token->type != WORD)
	{
		printf("bash: syntax error near '\\n'\n"); //mettre mon printf
		return (1);
	}
	return (0);
}