/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 21:49:03 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/20 11:25:48 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_match( char *str, char *pattern) 
{
    char *s_start;
    char *wildcard_prev;

    s_start = NULL;
    wildcard_prev = NULL;
    while (*str) 
	{
        // If the pattern character is '*' or the characters match
        if (*pattern == '*') 
        {
            // Remember this wildcard position in the pattern and the corresponding string position
            wildcard_prev = pattern++;
            s_start = str;
        } 
        else if (*pattern == *str)
        {
            // Characters match, move to the next characters in both the string and the pattern
            str++;
            pattern++;
        }
		else if (wildcard_prev != NULL) 
		{
            // Mismatch, but we have a previous '*' wildcard to backtrack to
            pattern = wildcard_prev + 1; // Move pattern pointer back to the character after '*'
            s_start++; // Move string pointer to the next character after the last match attempt
            str = s_start;  // Advance string pointer to the next character after the last match attempt
        } 
		else 
            return (FALSE);
    }
    // Skip over any remaining '*' wildcards in the pattern
    while (*pattern && *pattern == '*')
            pattern++;
    return (*pattern == '\0');
}

int get_wildcard(t_token_list **wildcard_list, char *pattern, t_minishell *info)
{
    DIR             *dir;
    struct dirent   *entry;
	t_token_list	*tmp;

	(*wildcard_list) = create_token();
	if (!(*wildcard_list))
	{
		info->last_exit_status = errno;
		return (info->last_exit_status);
	}
	tmp = (*wildcard_list);
	dir = opendir(".");
	if (!dir) 
	{
		info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "opendir: %s\n", strerror(errno));
		free_token_list((*wildcard_list));
        return (info->last_exit_status);
    }
	while (TRUE)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (!is_match(entry->d_name, pattern))
			continue ;
		tmp->next = new_token(entry->d_name, WORD);
		if (!tmp->next)
		{
			info->last_exit_status = errno;
			free_token_list((*wildcard_list)->next);
			(*wildcard_list)->next = NULL;
			break ;
		}
		tmp = tmp->next;
	}
	tmp = (*wildcard_list);
	(*wildcard_list) = (*wildcard_list)->next;
	if ((*wildcard_list) == NULL)
	{
		(*wildcard_list) = new_token(pattern, WORD);
		if (!(*wildcard_list))
		{
			info->last_exit_status = errno;
			free_token_list((*wildcard_list));
			return (info->last_exit_status);
		}//to verify about freeing 
	}
	free(tmp);
	if (closedir(dir) == -1)
		ft_fprintf(STDERR_FILENO, "closedir: %s\n", strerror(errno));
	return (OK);
}

t_token_list	*last_list_elem(t_token_list *list)
{
	if (list)
		while (list->next)
			list = list->next;
	return (list);
}

int	wildcard_handler(t_token_list **args, t_minishell *info, char runtime)
{
	t_token_list	tmp;
	t_token_list	*tmp_ptr;
	t_token_list	*wildcard_list;
	t_token_list	*last;

	tmp_ptr = &tmp;
	tmp_ptr->next = *args;
	*args = tmp_ptr;
	wildcard_list = NULL;
	while (tmp_ptr->next)
	{
		if (tmp_ptr->next->type == WORD && strchr(tmp_ptr->next->str, '*'))
		{
			if(get_wildcard(&wildcard_list, tmp_ptr->next->str, info) != OK)
				return (info->last_exit_status);
			last = last_list_elem(wildcard_list);
			last->next = tmp_ptr->next->next;
			free(tmp_ptr->next->str);
			free(tmp_ptr->next);
			tmp_ptr->next = wildcard_list;
			tmp_ptr = last;
		}
		else if (runtime && tmp_ptr->next->type == STDIN_HEREDOC)
			tmp_ptr = tmp_ptr->next->next;
		else
			tmp_ptr = tmp_ptr->next;
	}
	*args = (*args)->next;
	return (info->last_exit_status);
}
