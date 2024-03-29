#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "minishell.h"

int ft_pwd(void)
{
	char path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL)
	{
		ft_fprintf(STDERR_FILENO, "pwd: %s\n", strerror(errno));
		return (errno);
	}
	ft_printf("%s\n", path);
	return (OK);
}
