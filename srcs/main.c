/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 17:01:22 by bahn              #+#    #+#             */
/*   Updated: 2021/12/21 17:13:07 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	minishell_init(int argc, char *argv[], char *env[])
{
	if (argc != 1)
		exit(EXIT_FAILURE);
	(void)argv;
	g_data.org_env = env;
	g_data.envv = set_env(env);
	if (g_data.home_path == NULL)
		g_data.home_path = getcwd(NULL, 0);
	if (chdir(getenv("HOME")) == -1)
		exit(errno);
	g_data.command_fp = NULL;
	g_data.pipe = malloc(sizeof(int) * 2);
}

static	void	minishell_finalize()
{
	ft_lstclear(g_data.envv, free);
	split_free(g_data.argv);
	split_free(g_data.split_input);
	free(g_data.home_path);
	free(g_data.prompt);
}

int	main(int argc, char *argv[], char *env[])
{
	minishell_init(argc, argv, env);
	while (1)
	{
		if (minishell() != 0)
			break ;
	}
	minishell_finalize();
	system("leaks minishell > leaks_result && cat leaks_result && rm -rf leaks_result");
	return (0);
}
