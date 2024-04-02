#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "minishell.h"

t_token_list	*skip_parenthesis(t_token_list *token)
{
	unsigned int	scope;

	scope = 0;
	while (TRUE)
	{
		if (token->type == OPENPAR)
			scope++;
		else if (token->type == CLOSEPAR && --scope == 0)
			return (token);
		token = token->next;
	}
}

t_node	*new_tree_node(char type, unsigned int child_nb, unsigned int pipe_nb)
{
	t_node	*new_node;

	new_node = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!new_node)
	{
		ft_fprintf(STDERR_FILENO, "failed to allocate tree node [type=%d]: %s\n", type, strerror(errno));
		return (NULL);
	}
	new_node->type = type;
	new_node->child_nb = child_nb;
	new_node->pipe_nb = pipe_nb;
	if (child_nb)
	{
		new_node->child = (t_node **)ft_calloc(child_nb, sizeof(t_node *));
		if (!new_node->child)
		{
			ft_fprintf(STDERR_FILENO, "failed to allocate children array for tree node [type=%d]: %s\n", type, strerror(errno));
			free(new_node);
			return (NULL);
		}
	}
	return (new_node);
}

t_token_list	*search_operator(t_token_list *start, t_token_list *end)//end est l'element juste apres le dernier, ca peut etre NULL
{
	t_token_list	*token;
	t_token_list	*saved_token;

	saved_token = NULL;
	token = start;
	while (token != end)
	{
		if (token->type == OPENPAR)
			token = skip_parenthesis(token);
		else if (token->type == OR || token->type == AND)
			saved_token = token;
		token = token->next;
	}
	return (saved_token);
}

unsigned int	count_pipes(t_token_list *start, t_token_list *end)
{
	unsigned int	pipe_nb;

	pipe_nb = 0;
	while (start != end)
	{
		if (start->type == OPENPAR)
			start = skip_parenthesis(start);
		else if (start->type == PIPE)
			pipe_nb++;
		start = start->next;
	}
	return (pipe_nb);
}

void free_tree(t_node *node)
{
	unsigned int	i;
	
	if (!node)
		return ;
	if (node->child_nb)
	{
		i = 0;
		while (i < node->child_nb && node->child[i])
		{
			free_tree(node->child[i]);
			node->child[i] = NULL;
			i++;
		}
		free(node->child);
		node->child = NULL;
	}
	free_token_list(node->redi);
	free_token_list(node->args);
	free(node);
}

void display_tree(t_node *node)
{
	unsigned int	i;

	if (node->type == CMD)
	{
		ft_printf("CMD: \n");
		ft_printf("args: ");
		display_token_list(node->args);
		ft_printf("\n");
		ft_printf("redi: ");
		display_token_list(node->redi);
		ft_printf("\n");
	}
	else if (node->type == PIPE)
	{
		ft_printf("PIPE: ---> \n");
		i = 0;
		while (i < node->child_nb)
		{
			display_tree(node->child[i]);
			i++;
		}
		ft_printf("PIPE: <--- \n");
	}
	else if (node->type == OPENPAR)
	{
		ft_printf("OPERATOR: OPENPAR ---> \n");
		display_tree(node->child[0]);
		ft_printf("OPERATOR: OPENPAR <--- \n");
	}
	else
	{
		if (node->type == OR)
			ft_printf("OPERATOR: OR ---> \n");
		else if (node->type == AND)
			ft_printf("OPERATOR: AND ---> \n");
		display_tree(node->child[0]);
		display_tree(node->child[1]);
		if (node->type == OR)
			ft_printf("OPERATOR: OR <--- \n");
		else if (node->type == AND)
			ft_printf("OPERATOR: AND <--- \n");
	}
}

int	pipe_children(t_token_list *start, t_token_list *end, t_node *pipe_node)
{
	t_token_list	*token;
	int				i;

	token = start;
	i = 0;
	while (token != end)
	{
		if (token->type == OPENPAR)
			token = skip_parenthesis(token);
		else if (token->type == PIPE)
		{
			pipe_node->child[i] = tree_maker(start, token);
			if (!pipe_node->child[i])
				return (MALLOC_ERROR);
			token = token->next;
			start = token;
			i++;
		}
		else
			token = token->next;
	}
	pipe_node->child[i] = tree_maker(start, end);
	if (!pipe_node->child[i])
		return (MALLOC_ERROR);
	return (OK);
}

int	init_cmd_node(t_token_list *start, t_token_list *end, t_node *cmd_node)
{
	t_token_list	*tail_redi;
	t_token_list	*token;
	t_token_list	*tail_args;

	while (start != end)
	{
		if (start->type >= STDIN && start->type <= STDOUT_APPEND)
		{
			token = copy_token(start);
			if (!token)
				return (MALLOC_ERROR);
			if (cmd_node->redi == NULL)
				cmd_node->redi = token;
			else
				tail_redi->next = token;
			tail_redi = token;
			start = start->next;
			token = copy_token(start);
			if (!token)
				return (MALLOC_ERROR);
			tail_redi->next = token;
			tail_redi = token;
		}
		else if (start->type == WORD)
		{
			token = copy_token(start);
			if (!token)
				return (MALLOC_ERROR);
			if (cmd_node->args == NULL)
				cmd_node->args = token;
			else
				tail_args->next = token;
			tail_args = token;
		}
		else
		{
			ft_fprintf(STDERR_FILENO, "unexpected token type in CMD: %d\n", start->type);
			return (SYNTAX_ERROR);
		}
		start = start->next;
	}
	return (OK);
}


t_node	*tree_maker(t_token_list *start, t_token_list *end)
{
	t_node			*new_node;
	t_token_list	*tmp_token;
	unsigned int	pipe_nb;

	tmp_token = search_operator(start, end);
	if (tmp_token)
	{
		new_node = new_tree_node(tmp_token->type, 2, 0);
		if (!new_node)
			return (NULL);
		new_node->child[0] = tree_maker(start, tmp_token);
		if (!new_node->child[0])
		{
			free_tree(new_node);
			return (NULL);
		}
		new_node->child[1] = tree_maker(tmp_token->next, end);
		if (!new_node->child[1])
		{
			free_tree(new_node);
			return (NULL);
		}
		return (new_node);
	}
	pipe_nb = count_pipes(start, end);
	if (pipe_nb)
	{
		new_node = new_tree_node(PIPE, pipe_nb + 1, pipe_nb);
		if (!new_node)
			return (NULL);
		if (pipe_children(start, end, new_node) != OK)
		{
			free_tree(new_node);
			return (NULL);
		}
		return (new_node);
	}
	if (start->type == OPENPAR)
	{
		new_node = new_tree_node(tmp_token->type, 1, 0);
		if (!new_node)
			return (NULL);
		new_node->child[0] = tree_maker(start->next, skip_parenthesis(start));
		if (!new_node->child[0])
		{
			free_tree(new_node);
			return (NULL);
		}
		return (new_node);
	}
	new_node = new_tree_node(CMD, 0, 0);
	if (!new_node)
		return (NULL);
	if (init_cmd_node(start, end, new_node) != OK)
	{
		free_tree(new_node);
		return (NULL);
	}
	return (new_node);
}
