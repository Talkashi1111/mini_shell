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

#include "minishell.h"

t_token_list	*tokenizer_error(t_token_list *list, t_token_list *current_token, t_minishell *info)
{
	info->last_exit_status = errno;
	ft_fprintf(STDERR_FILENO, "bash: malloc: %s\n", strerror(errno));
	free_token_list(list);
	free(current_token);
	return (NULL);
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
t_token_list	*tokenizer(char *str, t_minishell *info)
{
	char			*end;
	t_token_list 	*tl[3];

	tl[HEAD] = NULL;
	while (str && *str != '\0')
	{
		skip_whitespace_start(&str);
		if (*str == '\0')
			break ;
		end = str;
		tl[TOKEN] = create_token();
		if (!tl[TOKEN])
			return (tokenizer_error(tl[HEAD], NULL, info));
		tl[TOKEN]->type = get_token_type(str);
		if (tl[TOKEN]->type == WORD)
			to_word_end(&end);
		else
			to_operator_end(&end);
		tl[TOKEN]->str = ft_substr(str, 0, end - str);
		if (!tl[TOKEN]->str)
			return (tokenizer_error(tl[HEAD], tl[TOKEN], info));
		add_back_tokenizer(tl);
		str = end;
	}
	return (tl[HEAD]);
}

