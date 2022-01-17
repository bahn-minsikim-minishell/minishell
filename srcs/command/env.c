/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:08:21 by bahn              #+#    #+#             */
/*   Updated: 2022/01/18 00:21:19 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_env(t_command *command)
{
	t_pipe	pipe_data;
	t_list	*ptr;

	(void)command;
	set_pipe(&pipe_data);
	ptr = g_data.lst_env;
	while (ptr != NULL)
	{
		ft_putstr_fd(((t_hash *)ptr->content)->key, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(((t_hash *)ptr->content)->value, 1);
		ptr = ptr->next;
	}
	return (release_pipe(&pipe_data));
}
