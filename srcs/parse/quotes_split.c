/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 21:22:04 by bahn              #+#    #+#             */
/*   Updated: 2022/01/25 19:13:27 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	arg_finder_to_list(t_list **list, char *sentence, char split_point)
{
	int		i;
	int		quotes_flag;
	char	*splitted;

	quotes_flag = 0;
	i = -1;
	while (sentence[++i] != '\0')
	{
		if (!quotes_flag)
		{
			if (sentence[i] == '\'' || sentence[i] == '\"')
			{
				if (quotes_flag)
					quotes_flag = 0;
				else
					quotes_flag = 1;
			}
			else if (sentence[i] == split_point && !quotes_flag)
			{
				splitted = ft_substr(sentence, 0, i);
				ft_lstadd_back(list, ft_lstnew(splitted));
				sentence += (++i);
				i = -1;
			}
		}
	}
	splitted = ft_substr(sentence, 0, i);
	ft_lstadd_back(list, ft_lstnew(splitted));
}

static void	arg_extractor_in_list(t_list *list, char **argv)
{
	int		i;
	t_list	*temp;

	i = -1;
	while (list != NULL)
	{
		temp = list->next;
		argv[++i] = (char *)list->content;
		free(list);
		list = temp;
	}
}

char	**quotes_split(char *sentence, char split_point)
{
	t_list	*list;
	char	**argv;

	list = NULL;
	arg_finder_to_list(&list, sentence, split_point);
	argv = ft_calloc(sizeof(char *), ft_lstsize(list) + 1);
	arg_extractor_in_list(list, argv);
	return (argv);
}
