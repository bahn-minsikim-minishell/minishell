/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 21:58:54 by bahn              #+#    #+#             */
/*   Updated: 2021/12/16 16:16:10 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*homepath_translator(char *path)
{
	char	*result;
	char	*temp;
	char	*temp2;

	if (path == NULL)
		return (NULL);
	if (*path == '~')
	{
		result = ft_strjoin(getenv("HOME"), "/");
		temp = ft_substr(path, 1, ft_strlen(path) - 1);
		temp2 = result;
		result = ft_strjoin(result, temp);
		free(temp2);
		free(temp);
		return (result);
	}
	else
		return (path);
}

int	minishell_cd(void)
{
	char	*temp;

	if (argv_counter(g_data.argv) > 2)
		printf("minishell: cd: too many arguments\n");
	else
	{
		if (g_data.argv[1] != NULL)
		{
			temp = g_data.argv[1];
			g_data.argv[1] = ft_strtrim(g_data.argv[1], "\'\"");
			free(temp);
		}
		if (g_data.argv[1] == NULL || !ft_strncmp(g_data.argv[1], "~", ft_strlen(g_data.argv[1])))
		{
			if (chdir(getenv("HOME")) == -1)
				printf("minishell: cd: %s: %s\n", getenv("HOME"), strerror(errno));
		}
		else if (g_data.argv[1] != NULL)
		{
			temp = homepath_translator(g_data.argv[1]);
			if (temp != NULL)
			{
				if (chdir(temp) == -1)
					printf("minishell: cd: %s: %s\n", g_data.argv[1], strerror(errno));
				if (temp != g_data.argv[1])
					free(temp);
			}
		}
	}
	free(g_data.current_path);
	g_data.current_path = getcwd(NULL, 0);
	return (SELF_PROC);
}

/********************************
 This Function is For execve()
********************************/
int	change_dir(void)
{
	if (argv_counter(g_data.argv) > 2)
		printf("minishell: cd: too many arguments\n");
	else if (!ft_strncmp(g_data.argv[1], "~", 1) || g_data.argv[1] == NULL)
	{
		if (chdir(g_data.home_path) == -1)
			exit(EXIT_FAILURE);
	}
	else if (g_data.argv[1] != NULL)
	{	
		if (chdir(g_data.argv[1]) == -1)
			exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_FAILURE);
	free(g_data.current_path);
	g_data.current_path = getcwd(NULL, 0);
	return (0);
}