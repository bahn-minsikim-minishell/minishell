/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 14:46:46 by bahn              #+#    #+#             */
/*   Updated: 2021/12/14 21:50:02 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_echo_for_execve(void)
{
	if (argv_counter(g_data.argv) == 1)
		return (0);
	else if (ft_strncmp(g_data.argv[1], "-n", ft_strlen(g_data.argv[1])) == 0)
	{
		if (argv_counter(g_data.argv) == 2)
			return (0);
		else if (g_data.argv[2][0] != '\'' && g_data.argv[2][0] != '\"')
			return (0);
		else
			return (then_input(2, g_data.argv[2][0]));
	}
	else if (g_data.argv[1][0] == '\'' || g_data.argv[1][0] == '\"')
	{
		return (then_input(1, g_data.argv[1][0]));
	}
	else // argv_counter > 2 && not option echo
		return (envvar_checker());
}

int	minishell_echo(void)
{
	int		i;
	int		j;
	char	*temp;
	
	i = 0;
	while (g_data.argv[++i] != NULL)
	{
		if (!ft_strncmp(g_data.argv[i], "-n", ft_strlen("-n")))
			continue ;
		else
		{
			temp = g_data.argv[i];
			g_data.argv[i] = ft_strtrim(g_data.argv[i], "\"\'");
			free(temp);
		}
		j = -1;
		while (g_data.argv[i][++j] != '\0')
		{
			if (g_data.argv[i][j] == '\\' && !ft_isalnum(g_data.argv[i][j + 1]))
				ft_putchar_fd(g_data.argv[i][++j], 1);
			else if (!ft_isalnum(g_data.argv[i][j]))
				continue ;
			else
				ft_putchar_fd(g_data.argv[i][j], 1);
		}
		if (g_data.argv[i + 1] != NULL)
			ft_putchar_fd(' ', 1);
	}
	if (g_data.argv[1] == NULL || ft_strncmp(g_data.argv[1], "-n", ft_strlen("-n")))
		ft_putchar_fd('\n', 1);
	return (-1);
}