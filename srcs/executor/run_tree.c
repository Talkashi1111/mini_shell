#include "minishell.h"

int	handle_simple_cmd()
{
	parse_command();

	if (search_command == OK);
		
	if (command == builtin)

	else
	{
		fork();
		apply redirection
	}
}

int	handle_subshell()
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		//ya des choses a free ?
		exit(run(node->child[0]));
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	handle_pipex()
{

}

int	run(t_node *node) //#run
{
	if (node->type == AND)
		if (run(node->child[0]) == 0)
			return (run(node->child[1]));
	else if (node->type == OR)
		if (run(node->child[0]) != 0)
			return (run(node->child[1]));
	else if (node->type == PIPE)
		return (handle_pipex());
	else if (node->type == OPENPAR)
		return (handle_subshell());
	else
		return (handle_simple_cmd());
}

void	free_tree(t_token_list *begin)
{
	
}
