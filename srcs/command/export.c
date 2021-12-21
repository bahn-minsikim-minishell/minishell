/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 14:57:34 by bahn              #+#    #+#             */
/*   Updated: 2021/12/18 01:25:02 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	export_format_checker(char *arg)
{
	char	*ptr;

	ptr = ft_strchr(arg, '=');
	if (ptr != NULL)
	{
		if (ptr == arg || ptr == (arg + ft_strlen(arg) - 1))
			return (1);
		else
			return (0);
	}
	else
		return (1);
}

static t_list	*export_dupl_checker(char *key)
{
	t_list	*ptr;

	ptr = *g_data.envv;
	while (ptr != NULL)
	{
		if (!ft_strncmp(((t_hash *)ptr->content)->key, key, ft_strlen(key) + 1))
			return (ptr);
		ptr = ptr->next;
	}
	return (NULL);
}

static	int		noargv_export(void)
{
	t_list	*ptr;

	ptr = *g_data.envv;
	while (ptr != NULL)
	{
		printf("declare -x %s=\"%s\"\n", ((t_hash *)ptr->content)->key, ((t_hash *)ptr->content)->value);
		ptr = ptr->next;
	}
	return (SELF_PROC);	
}

int	minishell_export(void)
{
	t_list	*ptr;
	t_hash	*hash;
	char	**temp;
	int		i;

	if (g_data.argv[1] == NULL)
		return (noargv_export());	
	i = 0;
	while (g_data.argv[++i] != NULL)
	{
		if (export_format_checker(g_data.argv[i]))
			continue ;
		temp = ft_split(g_data.argv[i], '=');
		hash = ft_calloc(sizeof(t_hash), 1);
		hash->key = temp[0];
		hash->value = temp[1];
		free(temp);
		ptr = export_dupl_checker(hash->key);
		if (ptr)
		{
			free(hash->key);
			free(((t_hash *)ptr->content)->value);
			((t_hash *)ptr->content)->value = hash->value;
			free(hash);
		}
		else
			ft_lstadd_back(g_data.envv, ft_lstnew(hash));
	}
	return (SELF_PROC);
}