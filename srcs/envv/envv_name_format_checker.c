/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envv_name_format_checker.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 22:36:12 by bahn              #+#    #+#             */
/*   Updated: 2022/01/17 22:41:43 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	envv_name_format_checker(char *key)
{
	int	i;

	if (ft_isdigit(*key))
		return (0);
	i = -1;
	while (key[++i] != '\0')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			break ;
	}
	if (key[i] != '\0')
		return (1);
	else
		return (0);
}
