#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "minishell.h"

/* 
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
} */


char *find_envp_arg(char *envp[], char *arg)
{
	int i;
	char *tmp;
	
	
	i = 0;
	while (envp[i])
	{
		tmp = ft_strjoin(arg, "=");
		if (ft_strnstr(envp[i], tmp, ft_strlen(tmp)) != NULL)
		{
			free(tmp);
			return (ft_strchr(envp[i], '=') + 1);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

int cd_builtin(char *args[], char *envp[])
{
	char *home_path;
	int chdir_ret;
	int err;

	home_path = find_envp_arg(envp, "HOME");
	ft_fprintf(STDOUT_FILENO, "home_path: %s\n", home_path);
	if (!args[1] || ft_strcnmp(args[1], "~", 1) == 0)
	{
		if (home_path == NULL)
		{
			ft_fprintf(STDERR_FILENO, "cd: HOME not set\n");
			return (USAGE_ERROR);
		}

		if (chdir(home_path) < 0)
		{
			ft_fprintf(STDERR_FILENO, "cd: %s: %s\n", home_path, strerror(errno));
			return (errno);
		}
	}
	else if (args[1])
	{
		if (ft_strncmp(args[1], "-", 1) == 0)
		{
			if (chdir(find_envp_arg(envp, "OLDPWD")) == -1)
			{
				ft_fprintf(STDERR_FILENO, "cd: OLDPWD not set\n");
				return (USAGE_ERROR);
			}
		}

		else
		{
			chdir_ret = chdir(args[1]);
			if (chdir_ret == -1)
			{
				ft_fprintf(STDERR_FILENO, "cd: %s: %s\n", args[1], strerror(errno));
				return (USAGE_ERROR);
			}
			else if (chdir_ret == 0)
			{
				ft_fprintf(STDOUT_FILENO, "please add me to the OLDPWD\n");
				return (OK);
/* 				if (!set_env_var("OLDPWD"))//to be implemented
					return (INIT_ERROR); */
			}
		}
	}
	return (OK);
}


