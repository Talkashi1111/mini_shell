#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "minishell.h"

int	cd_builtin(char *path)
{
	int	err;
	if (chdir(path) == -1)
	{
		err = errno;
		ft_fprintf(STDERR_FILENO, "cd: %s: %s\n", path, strerror(err));
		return (err);
	}
	return (OK);
}