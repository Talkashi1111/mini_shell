/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_tree_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:12:51 by achappui          #+#    #+#             */
/*   Updated: 2024/04/06 19:12:51 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*handle_operator(t_token_list *start, t_token_list *end, t_token_list *tmp_token, t_minishell *info)
{
	t_node			*new_node;

	new_node = new_tree_node(tmp_token->type, 2, 0, info);
	if (!new_node)
		return (NULL);
	new_node->child[0] = tree_maker(start, tmp_token, info);
	if (!new_node->child[0])
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "bash: malloc: %s\n", strerror(errno));
		free_tree(new_node);
		return (NULL);
	}
	new_node->child[1] = tree_maker(tmp_token->next, end, info);
	if (!new_node->child[1])
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "bash: malloc: %s\n", strerror(errno));
		free_tree(new_node);
		return (NULL);
	}
	return (new_node);
}

t_node	*handle_pipe(t_token_list *start, t_token_list *end, unsigned int	pipe_nb, t_minishell *info)
{
	t_node			*new_node;

	new_node = new_tree_node(PIPE, pipe_nb + 1, pipe_nb, info);
	if (!new_node)
		return (NULL);
	if (pipe_children(start, end, new_node, info) != OK)
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "bash: malloc: %s\n", strerror(errno));
		free_tree(new_node);
		return (NULL);
	}
	return (new_node);
}

t_node	*handle_parenthesis(t_token_list *start, t_minishell *info)
{
	t_node			*new_node;

	new_node = new_tree_node(OPENPAR, 1, 0, info);
	if (!new_node)
		return (NULL);
	new_node->child[0] = tree_maker(start->next, skip_parenthesis(start), info);
	if (!new_node->child[0])
	{
		info->last_exit_status = errno;
		ft_fprintf(STDERR_FILENO, "bash: malloc: %s\n", strerror(errno));
		free_tree(new_node);
		return (NULL);
	}
	return (new_node);
}

t_node	*handle_cmd(t_token_list *start, t_token_list *end, t_minishell *info)
{
	t_node			*new_node;

	new_node = new_tree_node(CMD, 0, 0, info);
	if (!new_node)
		return (NULL);
	if (init_cmd_node(start, end, new_node, info) != OK)
	{
		free_tree(new_node);
		return (NULL);
	}
	return (new_node);
}

void    fill_tab(char *tab, unsigned int size, uintptr_t n)
{
    char            *heredoc_path;
    unsigned int    i;
    
    heredoc_path = HEREDOC_PATH;
    i = -1;
    while (heredoc_path[++i])
        tab[i] = heredoc_path[i];
    tab[i++] = '/';
    tab[i] = '.';
    tab[size] = '\0';
    while (tab[--size] != '.')
    {
        tab[size] = n % 10 + 48;
        n /= 10;
    }
}

char    *ft_itoa_heredoc(uintptr_t n)
{
    uintptr_t    tab;
    unsigned int size;

    tab = n;
    size = 1;
    while (tab >= 10 && size++)
        tab /= 10;
    size += ft_strlen(HEREDOC_PATH) + 2;
    tab = (uintptr_t)malloc((size + 1) * sizeof(char));
    if (!tab)
        return (NULL);
    fill_tab((char *)tab, size, n);
    return ((char *)tab);
}