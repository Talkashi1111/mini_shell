/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 13:33:27 by achappui          #+#    #+#             */
/*   Updated: 2024/03/22 13:45:55 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void			free_token_list(t_token_list *begin);
t_token_list	*tokenizer(char *str);

void	display_token_list(t_token_list *begin)
{
	while (begin)
	{
		printf("\"%s\" ", begin->str);
		begin = begin->next;
	}
	printf("\n");
}

int	main(int argc, char *argv[])
{
	t_token_list	*token_list;

	if (argc != 2)
	{
		printf("need 1 argument\n");
		return (1);
	}
	token_list = tokenizer(argv[1]);
	if (token_list == NULL) ///je free pas encore la liste quand ya une erreur de malloc
		return (1);
	display_token_list(token_list);
	free_token_list(token_list);
	return (0);
}