/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 21:49:03 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/02 21:56:15 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "minishell.h"

#define TRUE 1
#define FALSE 0


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

t_token_list *get_wildcard(char *pattern)
{
    DIR             *dir;
    struct dirent   *entry;
    t_token_list    *head;
	t_token_list	*tmp;

	head = create_token();
	if (!head)
		return (NULL);
	tmp = head;
	dir = opendir(".");
	if (!dir) 
	{
        ft_fprintf(STDERR_FILENO, "opendir\n", strerror(errno));
		free_token_list(head);
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
			free_token_list(head->next);
			head->next = NULL;
			break ;
		}
		tmp = tmp->next;
	}
	tmp = head;
	head = head->next;
	free(tmp);
	if (closedir(dir) == -1)
		ft_fprintf(STDERR_FILENO, "closedir: %s\n", strerror(errno));
    return (head);
}