
#include "minishell.h"

void	display_token_list(t_token_list *token)
{
	if (!token)
	{
		ft_printf("empty token list\n");
		return ;
	}
	while (token)
	{
		ft_printf("\"%s\" TYPE: %i ", token->str, token->type);
		token = token->next;
	}
	ft_printf("\n");
}

void display_tree_recursive(t_node *node, int depth, int isLast, int *siblings) {
    if (!node)
		return;
    for (int i = 0; i < depth - 1; ++i)
        ft_printf("%s   ", siblings[i] ? "│" : " ");
    if (depth > 0)
        ft_printf("%s── ", isLast ? "└" : "├");
    if (node->type == CMD)
	{
        ft_printf("CMD\n");
		// Ensure correct indentation for args and redi
		for (int i = 0; i < depth; ++i)
			ft_printf("%s   ", siblings[i] ? "│" : " ");
		ft_printf("  args: "); // Indentation for args
		display_token_list(node->args);
		for (int i = 0; i < depth; ++i)
			ft_printf("%s   ", siblings[i] ? "│" : " ");
		ft_printf("  redi: "); // Indentation for redi
		display_token_list(node->redi);
    }
	else
		ft_printf("%s\n", node_type_to_string(node->type));

    // Update the siblings array to keep track of which levels have remaining siblings
    if (depth > 0)
        siblings[depth - 1] = !isLast;

    // Recursively print child nodes
    for (unsigned int i = 0; i < node->child_nb; ++i)
        display_tree_recursive(node->child[i], depth + 1, i == node->child_nb - 1, siblings);

    // Reset the siblings array back
    if (depth > 0)
        siblings[depth - 1] = 0;
}

void	display_tree(t_node *node)
{
    int	siblings[128];
	int	i;

	i = 0;
	while (i < 128)
	{
		siblings[i] = 0;
		i++;
	}
    display_tree_recursive(node, 0, 1, siblings);
}