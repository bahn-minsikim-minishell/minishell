/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execfile_finder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 11:23:42 by bahn              #+#    #+#             */
/*   Updated: 2021/12/28 12:18:30 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*execfile_finder(char *command)
{
	int			i;
	char		*compose;
	struct stat	buf;

	i = -1;
	while (g_data.envv_path[++i] != NULL)
	{
		compose = ft_strjoin(g_data.envv_path[i], command);
		if (!stat(compose, &buf))
			return (compose);
		free(compose);
	}
	return (NULL);
}