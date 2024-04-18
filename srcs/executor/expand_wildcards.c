/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 21:49:03 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/17 22:52:43 by achappui         ###   ########.fr       */
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

t_token_list *get_wildcard(char *pattern, t_minishell *info)
{
    DIR             *dir;
    struct dirent   *entry;
    t_token_list    *head;
	t_token_list	*tmp;

	head = create_token();
	if (!head)
	{
		info->last_exit_status = MALLOC_ERROR;
		return (NULL);
	}
	tmp = head;
	dir = opendir(".");
	if (!dir) 
	{
		info->last_exit_status = errno;
        ft_fprintf(STDERR_FILENO, "opendir: %s\n", strerror(errno));
		free_token_list(head, 0, info);
        return (NULL);
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
			info->last_exit_status = MALLOC_ERROR;
			free_token_list(head->next, 0, info);
			head->next = NULL;
			break ;
		}
		tmp = tmp->next;
	}
	tmp = head;
	head = head->next;
	if (head == NULL)
	{
		head = new_token(pattern, WORD);
		if (!head)
		{
			info->last_exit_status = MALLOC_ERROR;
			free_token_list(head, 0, info);
			return (NULL);
		}//to verify about freeing 
	}
	free(tmp);
	if (closedir(dir) == -1)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "closedir: %s\n", strerror(errno));
	}
	return (head);
}

t_token_list	*last_list_elem(t_token_list *list)
{
	if (list)
		while (list->next)
			list = list->next;
	return (list);
}

int	wildcard_handler(t_token_list **token, t_minishell *info)
{
	t_token_list	*prev;
	t_token_list	*tmp;
	t_token_list	*last;

	prev = create_token();
	if (!prev)
	{
		info->last_exit_status = MALLOC_ERROR;
		return (MALLOC_ERROR);
	}
	prev->next = *token;
	*token = prev;
	while (prev->next)
	{
		if (prev->next->type == WORD && strchr(prev->next->str, '*'))
		{
			tmp = get_wildcard(prev->next->str, info);
			if(info->last_exit_status != OK)
			{
				prev = *token;
				*token = (*token)->next;
				free(prev);
				return (info->last_exit_status);
			}
			if (tmp == NULL)
			{
				tmp = prev->next;
				prev->next = prev->next->next;
				free(tmp->str);
				free(tmp);
			}
			else
			{
				last = last_list_elem(tmp);
				last->next = prev->next->next;
				free(prev->next->str);
				free(prev->next);
				prev->next = tmp;
				prev = last;
			}
		}
		else
			prev = prev->next;
	}
	prev = *token;
	*token = (*token)->next;
	free(prev);
	return (info->last_exit_status);
}