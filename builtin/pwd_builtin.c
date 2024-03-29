#include <errno.h>
#include <unistd.h>
#include "minishell.h"

int ft_pwd(char *args[], char *envp[])
{
	char path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL)
	{
		ft_fprintf(STDERR_FILENO, "pwd: %s\n", strerror(errno));
		return (errno);
	}
	ft_fprintf(STDOUT_FILENO, "%s\n", path);
	return (OK);
}
