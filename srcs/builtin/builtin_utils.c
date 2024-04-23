/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:44:31 by achappui          #+#    #+#             */
/*   Updated: 2024/04/23 10:40:12 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pfunc	is_builtin(char *str, t_builtin *builtin)
{
	unsigned int	i;
	unsigned int	name_len;

	if (!str)
		return (NULL);
	i = 0;
	while (builtin[i].name)
	{
		name_len = ft_strlen(builtin[i].name);
		if (ft_strncmp(str, builtin[i].name, name_len) == 0 && str[name_len] == '\0')
			return (builtin[i].func);
		i++;
	}
	return (NULL);
}

char	*ft_strnstr_env(char *env, char *var_name, unsigned int var_name_len)
{
	while (var_name_len)
	{
		if (*env == '=' || *env != *var_name)
			return (NULL);
		env++;
		var_name++;
		var_name_len--;
	}
	if (*env == '=')
		return (env + 1);
	return (NULL);
}

char	*find_envp_arg(char *envp[], char *var_name, unsigned int var_name_len)
{
	char	*value;
	int		i;

	i = 0;
	while (envp[i])
	{
		value = ft_strnstr_env(envp[i], var_name, var_name_len);
		if (value)
			return (value);
		i++;
	}
	return (NULL);
}

int	add_envp(t_minishell *info, char *new_entry)
{
	char	**envp_new;
	int		i;

	i = count_args(info->envp);
	envp_new = ft_calloc(i + 2, sizeof(char *));
	if (!envp_new)
		return (MALLOC_ERROR);
	i = 0;
	while (info->envp[i])
	{
		envp_new[i] = info->envp[i];
		i++;
	}
	envp_new[i] = new_entry;
	free(info->envp);
	info->envp = envp_new;
	return (OK);
}

int	update_or_add_envp(t_minishell *info, char *str, char *new_val)
{
	int		i;
	char	*new_entry;

	new_entry = ft_strjoin(str, new_val);
	if (!new_entry)
		return (info->last_exit_status = errno);
	i = 0;
	while (info->envp[i])
	{
		if (ft_strnstr(info->envp[i], str, ft_strlen(str)) != NULL)
		{
			free(info->envp[i]);
			info->envp[i] = new_entry;
			info->last_exit_status = OK;
			return (OK);
		}
		i++;
	}
	if (add_envp(info, new_entry) == MALLOC_ERROR)
	{
		free(new_entry);
		info->last_exit_status = MALLOC_ERROR;
		return (MALLOC_ERROR);
	}
	return (OK);
}
