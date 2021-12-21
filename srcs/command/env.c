/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:08:21 by bahn              #+#    #+#             */
/*   Updated: 2021/12/21 15:21:32 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		minishell_env(void)
{
	t_list	*ptr;

	ptr = *g_data.envv;
	while (ptr != NULL)
	{
		ft_putstr_fd(((t_hash *)ptr->content)->key, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(((t_hash *)ptr->content)->value, 1);
		ptr = ptr->next;
	}
	return (SELF_PROC);
}

char	*env_getvalue(char *key)
{
	t_list	*ptr;

	ptr = *g_data.envv;
	while (ptr != NULL)
	{
		if (!ft_strncmp(((t_hash *)ptr->content)->key, key, ft_strlen(key) + 1))
			return (((t_hash *)ptr->content)->value);
		ptr = ptr->next;
	}
	return (NULL);
}

void	env_chararr_converter(void)
{
	t_list	*ptr;
	int		i;
	char	*temp;

	ptr = *g_data.envv;
	i = -1;
	g_data.org_env = ft_calloc(sizeof(char *), ft_lstsize(*g_data.envv) + 1);
	while (ptr != NULL)
	{
		temp = ft_strjoin(((t_hash *)ptr->content)->key, "=");
		g_data.org_env[++i] = ft_strjoin(temp, ((t_hash *)ptr->content)->value);
		free(temp);
	}
	g_data.org_env[i] = NULL;
}

t_list	**set_env(char *env[])
{
	t_list	**lst;
	t_hash	*hash;
	char	**temp;
	int		env_len;
	int		i;

	i = -1;
	env_len = 0;
	while (env[++i] != NULL)
		env_len++;
	lst = ft_calloc(sizeof(t_list *), env_len);
	i = -1;
	while (env[++i] != NULL)
	{
		temp = ft_split(env[i], '=');
		hash = ft_calloc(sizeof(t_hash), 1);
		hash->key = temp[0];
		hash->value = temp[1];
		free(temp);
		ft_lstadd_back(lst, ft_lstnew(hash));
	}
	return (lst);
}
