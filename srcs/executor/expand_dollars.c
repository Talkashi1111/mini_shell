/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:43:53 by achappui          #+#    #+#             */
/*   Updated: 2024/04/16 17:04:13 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdlib.h>
#include <stdbool.h>
#include "minishell.h"
#include <stdio.h>

long long	with_dollar(char **str, char **seq, unsigned int *len, bool *to_free, t_minishell *info)
{
	char			*delimiters;
	unsigned int	varname_len;
	long long		i;
	
	i = 0;
	delimiters = " \t\n\v\f\r'*\"$\0";
	if ((*str)[i + 1] == '?')
	{
		*seq = ft_itoa(info->last_exit_status);
		if (!*seq)
			return (-1);
		*to_free = TRUE;
		i = ft_strlen(*seq);
		*str += 2; //why 1 + 1? //to remember that one is for $ and one is for ? though its not a big deal I was hesitating
	}
	else
	{
		varname_len = 0;
		while (ft_strchr(delimiters, (*str + 1)[varname_len]) == NULL)
			varname_len++;
		*seq = find_envp_arg(info->envp, *str + 1 , varname_len);
		if (*seq)
			i = ft_strlen(*seq);
		*str += varname_len + 1;
	}
	*len += i;
	return (i);
}

long long no_dollar(char **str, char **seq, unsigned int *len)
{
	long long	i;

	*seq = *str;
	i = 0;
	while ((*str)[i] != '$' && (*str)[i] != '\0')
	{
		if ((*str)[i] == '\'')
			i += to_end_of_quote(*str + i) - (*str + i); 
		i++;
	}
	*str += i;
	*len += i;
	return (i);
}


char	*expand_dollar(char *str, unsigned int len, t_minishell *info)
{
	char		*new_str;
	char		*seq;
	bool		to_free;
	long long	i;

	to_free = FALSE;
	new_str = NULL;
	if (str[0] == '$')
		i = with_dollar(&str, &seq, &len, &to_free, info);
	else
		i = no_dollar(&str, &seq, &len);
	if (i == -1) //why -3? Best practice is avoid magic numbers //wasnt supposed to be a magic number I was needing it in an older version
		return (NULL);
	if (*str == '\0')
	{
		new_str = (char *)malloc((len + 1) * sizeof(char));
		new_str[len] = '\0';
		if (!new_str)
		{
			if (to_free)
				free(seq);
			return (NULL); // forgot to free seq //yes
		}
	}
	else
		new_str = expand_dollar(str, len, info);
	if (new_str)
		while (i)
			new_str[--len] = seq[--i]; //potential NULL pointer dereference //yes if expand dollars returns NULL
	if (to_free)
		free(seq);
	return (new_str);
}

char	expand_dollars(t_token_list **args, t_minishell *info)
{
	t_token_list	tmp;
	t_token_list	*tmp_ptr;
	t_token_list	*to_free;

	tmp_ptr = &tmp;
	tmp_ptr->next = *args;
	*args = tmp_ptr; //why do we need to do this? //to simplify the logic, meaning less line to write, easier error management, less code else we would have to deal with 2 cases
	while (tmp_ptr->next)
	{
		tmp_ptr->next->str = expand_dollar(tmp_ptr->next->str, 0, info);
		if (!tmp_ptr->next->str)
		{
			info->last_exit_status = MALLOC_ERROR;
			*args = (*args)->next;
			return (MALLOC_ERROR); //bug here, forgot to promote args to next //yes
		}
		else if (tmp_ptr->next->str[0] == '\0')
		{
			to_free = tmp_ptr->next;
			tmp_ptr->next = tmp_ptr->next->next;
			free(to_free->str);
			free(to_free);
		}
		else
			tmp_ptr = tmp_ptr->next;
	}
	*args = (*args)->next; //why do we need to do this? //so that *args gets updated correctly
	return (OK);
}
