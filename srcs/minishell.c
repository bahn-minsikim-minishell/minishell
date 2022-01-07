/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:03:56 by bahn              #+#    #+#             */
/*   Updated: 2022/01/07 22:59:41 by bahn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_flag(t_list *i_list)
{
	t_list	*list;
	t_command	*content;
	t_command	*next_con;

	list = i_list;
	while (list->next)
	{
		content = (t_command *)list->content;
		next_con = (t_command *)list->next->content;
		if (ft_strncmp(content->redirect, ";", 2) == 0)
		{
			content->next_flag = 1;
			next_con->pre_flag = 1;
		}
		if (ft_strncmp(content->redirect, "|", 2) == 0)
		{
			content->next_flag = 2;
			next_con->pre_flag = 2;
		}
		if (ft_strncmp(content->redirect, ">", 2) == 0)
		{
			content->next_flag = 3;
			next_con->pre_flag = 3;
		}
		if (ft_strncmp(content->redirect, ">>", 3) == 0)
		{
			content->next_flag = 4;
			next_con->pre_flag = 4;
		}
		list = list->next;
	}
}

void	ft_pipe(t_list	**list)
{
	int		**fd;
	int		i;
	int		size;
	t_command	*content;
	pid_t		pid;
	int			status;

	size = ft_lstsize(g_data.commands);
	fd = malloc(sizeof(int *) * size);
	i = -1;
	while (++i < size)
	{
		fd[i] = malloc(sizeof(int) * 2);
	}
	i = -1;
	while (++i < size && (*list)->content != NULL)
	{
		content = (*list)->content;
		pipe(fd[i]);
		pid = fork();
		if (pid == 0) // son
		{
			if (content->pre_flag == 2) // | argv
			{
				dup2(fd[i - 1][0], STDIN_FILENO);
			}
			if (content->next_flag == 2) // argv |
			{
				dup2(fd[i][1], STDOUT_FILENO);
			}
			if (content->next_flag == 3) // argv >
			{
				fd[i][1] = open(((t_command *)(*list)->next->content)->argv[0], O_WRONLY | O_CREAT, 0644); // S_IROTH : 개인에게 읽기권한
				dup2(fd[i][1], STDOUT_FILENO);
				// close(fd[i][1]);
			}
			if (content->next_flag == 4) // argv >>
			{
				fd[i][1] = open(((t_command *)(*list)->next->content)->argv[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
				dup2(fd[i][1], STDOUT_FILENO);
			}
			to_execve_2((*list)->content);
		}
		else // parent
		{
			wait(&status);
			if (content->pre_flag == 2)
				close(fd[i - 1][0]);
			if (content->next_flag == 2)
				close(fd[i][1]);
			g_data.status = WEXITSTATUS(g_data.status);
			if (g_data.status == 127)
				printf("minishell: %s: command not found\n", ((t_command *)(*list)->content)->argv[0]);
		}
		if ((*list)->next)
			*list = (*list)->next;
	}
}

int	minishell(char *input)
{
	t_list		*list;
	t_command	*cmd;
	int			i;

	more_input(&input);
	// incorrected history if input is not enclosed quotes and backslash
	add_history(input);
	// Remove Enclosed quotes
	// input_split(&g_data.commands, ft_strtrim(input, " "));
	input_split2(&g_data.commands, ft_strtrim(input, " "));
	refact_by_redirection(&g_data.commands);
	free(input);
	set_flag(g_data.commands);
	list = g_data.commands;
	while (list != NULL)
	{
		cmd = list->content;
		printf("command : %s\n", cmd->argv[0]);
		i = 0;
		while (cmd->argv[++i] != NULL)
			printf("argv[%d] : %s\n", i, cmd->argv[i]);
		printf("redirect : %s\n", cmd->redirect);
		if (cmd->redirect != NULL)
		{
			ft_pipe(&(list));
		}
		else
		{
			parsing(cmd);
			if (!cmd->bulit_in_flag)
				to_execve(cmd);
		}
		list = list->next;
	}
	ft_lstclear(&g_data.commands, command_free);
	return (0);
}
