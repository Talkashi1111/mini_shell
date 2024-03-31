#include "minishell.h"

int ft_echo(char *args[], char **envp[])
{
	int i;
	int option_n;
	int n_args;

	(void)envp;
	n_args = count_args(args);
	option_n = FALSE;
	i = 1;
	if (n_args > 1 && ft_strncmp(args[1], "-n", sizeof("-n")) == 0)
	{
		option_n = TRUE;
		i++;
	}
	while (i < n_args)
	{
		ft_printf("%s", args[i]);
		if (i < n_args - 1)
			ft_printf(" ");
		i++;
	}
	if (!option_n)
		ft_printf("\n");
	return (OK);
}
