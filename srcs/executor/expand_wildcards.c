/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 21:49:03 by tkashi            #+#    #+#             */
/*   Updated: 2024/04/21 22:15:51 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	assign_ptrs(char **str1, char *str2, char **pattern1, char *pattern2)
{
	*str1 = str2;
	*pattern1 = pattern2;
}

int	is_match( char *str, char *pattern)
{
	char	*s_start;
	char	*wildcard_prev;

	assign_ptrs(&s_start, NULL, &wildcard_prev, NULL);
	while (*str)
	{
		if (*pattern == '*')
		{
			wildcard_prev = pattern++;
			s_start = str;
		}
		else if (*pattern == *str)
			assign_ptrs(&str, str + 1, &pattern, pattern + 1);
		else if (wildcard_prev != NULL)
		{
			pattern = wildcard_prev + 1;
			s_start++;
			str = s_start;
		}
		else
			return (FALSE);
	}
	while (*pattern && *pattern == '*')
		pattern++;
	return (*pattern == '\0');
}

void	ft_itarate_find(DIR *dir, char *pattern, t_token_list **wildcard_list)
{
	struct dirent	*entry;
	t_token_list	*tmp;

	tmp = (*wildcard_list);
	while (TRUE)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (!is_match(entry->d_name, pattern))
			continue ;
		tmp->next = new_token(entry->d_name, WORD);
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	tmp = (*wildcard_list);
	(*wildcard_list) = (*wildcard_list)->next;
	free(tmp);
}

int	get_wildcard(t_token_list **wildcard_list, char *pattern, t_minishell *info)
{
	DIR				*dir;

	(*wildcard_list) = create_token();
	if (!(*wildcard_list))
	{
		info->last_exit_status = errno;
		return (info->last_exit_status);
	}
	dir = opendir(".");
	if (!dir)
	{
		info->last_exit_status = errno;
		ft_fprintf(info->saved_streams[1], "opendir: %s\n", strerror(errno));
		free_token_list((*wildcard_list));
		return (info->last_exit_status);
	}
	ft_itarate_find(dir, pattern, wildcard_list);
	if (no_path_found_and_closedir(info, wildcard_list, pattern, dir) != OK)
		return (info->last_exit_status);
	return (OK);
}

int	wildcard_handler(t_token_list **args, t_minishell *info, char runtime)
{
	t_token_list	tmp;
	t_token_list	*tmp_ptr;

	tmp_ptr = &tmp;
	tmp_ptr->next = *args;
	*args = tmp_ptr;
	while (tmp_ptr->next)
	{
		if (tmp_ptr->next->type == WORD && strchr(tmp_ptr->next->str, '*'))
		{
			if (assign_to_list_wildcard(info, tmp_ptr) != OK)
				return (info->last_exit_status);
		}
		else if (runtime && tmp_ptr->next->type == STDIN_HEREDOC)
			tmp_ptr = tmp_ptr->next->next;
		else
			tmp_ptr = tmp_ptr->next;
	}
	*args = (*args)->next;
	return (info->last_exit_status);
}
