#include "minishell.h"
#include <stdlib.h>

t_token_list	*skip_parenthesis(t_token_list *token)
{
	unsigned int	scope;

	scope = 0;
	while (1)
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

	new_node = (t_node *)malloc(sizeof(t_node));
	new_node->type = type;
	new_node->args = NULL;
	new_node->pipes = NULL;
	new_node->redi = NULL;
	new_node->child = NULL;
	new_node->pipe_nb = 0;
	if (child_nb)
	{
		new_node->child = (t_node **)malloc(child_nb * sizeof(t_node *));
	}
	if (pipe_nb)
	{
		new_node->pipes = (int**)malloc(pipe_nb * sizeof(int[2])); //a verifier si la syntaxe est correcte
		new_node->pipe_nb = pipe_nb;
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

void	handle_pipes(t_token_list *start, t_token_list *end, t_node *pipe_node)
{
	t_token_list	*token;

	token = start;
	while (token != end)
	{
		if (token->type == OPENPAR)
			token = skip_parenthesis(token);
		else if (token->type == PIPE)
		{
			tree_maker(start, token);
			start = token->next;
		}
	}
}

void	fill_command(t_token_list *start, t_token_list *end, t_node *cmd_node)
{
	while (start != end)
	{
		if (start->type >= STDIN && start->type <= STDOUT_APPEND)
		{
			add_back(start, cmd_node->redi); //recuperer le addback
			start = start->next;
			add_back(start, cmd_node->redi); //recuperer le addback
		}
		else if (start->type == WORD)
			add_back(start, cmd_node->args);
		start = start->next;
	}
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
		new_node->child[0] = tree_maker(start, tmp_token);
		new_node->child[1] = tree_maker(tmp_token->next, end);
		return (new_node);
	}
	pipe_nb = count_pipes(start, end);
	if (pipe_nb)
	{
		new_node = new_tree_node(PIPE, pipe_nb + 1, pipe_nb);
		handle_pipes(start, end, new_node);
		return (new_node);
	}
	if (start->type == OPENPAR)
	{
		new_node = new_tree_node(tmp_token->type, 1, 0);
		new_node->child[0] = tree_maker(start->next, skip_parenthesis(start));
		return (new_node);
	}
	new_node = new_tree_node(CMD, 0, 0);
	fill_command(start, end, new_node);
	return (new_node);
}
