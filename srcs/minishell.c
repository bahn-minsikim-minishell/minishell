/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsikim <minsikim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:03:56 by bahn              #+#    #+#             */
/*   Updated: 2022/01/20 11:13:50 by minsikim         ###   ########.fr       */
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
		if (ft_strncmp(content->redirect, "<", 2) == 0)
		{
			content->next_flag = 5;
			next_con->pre_flag = 5;
		}
		if (ft_strncmp(content->redirect, "<<", 2) == 0)
		{
			content->next_flag = 6;
			next_con->pre_flag = 6;
		}
		list = list->next;
	}
}

void	if_pipe(t_list *list, int **fd, int i)
{
	if (((t_command *)(list)->content)->next_flag == 2) // argv(입구-출력) | (출구-입력) // 알림장: >만 나왔을때 섹폴뜸; -> ok
	{
		dup2(fd[i][1], STDOUT_FILENO);
	}
}

void	if_flag_right(t_list *list, t_command *exe, int **fd, int i)
{
	if (((t_command *)(list)->content)->next_flag == 3 || ((t_command *)(list)->content)->next_flag == 4) // > or >>
	{
		exe = list->content;
		while (((t_command *)(list)->content)->next_flag == 3 || ((t_command *)(list)->content)->next_flag == 4) // > 또는 >>
		{
			if (((t_command *)(list)->content)->next_flag == 3)
				fd[i][1] = open(((t_command *)(list)->next->content)->argv[0], O_RDWR | O_CREAT | O_TRUNC, 0644); // S_IROTH : 개인에게 읽기권한 , 0644: 소유자-WR,RD 그룹,개인-RD
			else
				fd[i][1] = open(((t_command *)(list)->next->content)->argv[0], O_RDWR | O_CREAT | O_APPEND, 0644);
			dup2(fd[i][1], STDOUT_FILENO);
			close(fd[i][1]);
			if (((t_command *)(list)->next->content)->next_flag != 3  && ((t_command *)(list)->next->content)->next_flag != 4) // aa > bb > cc
			{
				break ;
			}
			list = (list)->next;
		}
		to_execve_2(exe);
	}
}

void	if_flag_right_no_exe(t_list *list, int **fd, int i)
{
	if (((t_command *)(list)->content)->next_flag == 3 || ((t_command *)(list)->content)->next_flag == 4) // > or >>
	{
		while (((t_command *)(list)->content)->next_flag == 3 || ((t_command *)(list)->content)->next_flag == 4) // > 또는 >>
		{
			if (((t_command *)(list)->content)->next_flag == 3)
				fd[i][1] = open(((t_command *)(list)->next->content)->argv[0], O_RDWR | O_CREAT | O_TRUNC, 0644); // S_IROTH : 개인에게 읽기권한 , 0644: 소유자-WR,RD 그룹,개인-RD
			else
				fd[i][1] = open(((t_command *)(list)->next->content)->argv[0], O_RDWR | O_CREAT | O_APPEND, 0644);
			dup2(fd[i][1], STDOUT_FILENO);
			close(fd[i][1]);
			if (((t_command *)(list)->next->content)->next_flag != 3  && ((t_command *)(list)->next->content)->next_flag != 4) // aa > bb > cc
			{
				break ;
			}
			list = (list)->next;
		}
	}
}

void	if_flag_left(t_list *list, t_command *exe, int **fd, int *i)
{
	if (((t_command *)(list)->content)->next_flag == 5) // flag <
	{
		exe = list->content;
		// printf("test_argv:%s\n", exe->argv[0]);
		while (((t_command *)(list)->content)->next_flag == 5)
		{
			fd[*i][1] = open(((t_command *)(list)->next->content)->argv[0], O_RDONLY, 0644);
			dup2(fd[*i][1], STDIN_FILENO);
			close(fd[*i][1]);
			if (((t_command *)(list)->next->content)->next_flag == 3 || ((t_command *)(list)->next->content)->next_flag == 4) // (~) < (~) > or >>
			{
				list = (list)->next;
				(*i)++;
				if_flag_right_no_exe(list, fd, *i);
				// printf("test_argv:%s\n", exe->argv[0]); // ???
				to_execve_2(exe);
			}
			if (((t_command *)(list)->next->content)->next_flag != 5) // aa > bb > cc
			{
				break ;
			}
			list = (list)->next;
		}
		// printf("test_argv:%s\n", exe->argv[0]);
		to_execve_2(exe);
	}
}

void	if_flag_d_left(t_list *list, t_command *exe, t_pipe pip, int *i)
{
	char *temp;
	int		out_fd;
	
	if (((t_command *)(list)->content)->next_flag == 6) // <<
	{
		
		out_fd = dup(STDOUT_FILENO);
		pip.pid = fork();
		if (pip.pid == 0)
		{
			pip.fd[*i][1] = open("temp", O_WRONLY | O_CREAT | O_APPEND, 0644);
			while (1)
			{
				temp = readline("> ");
				if (ft_strncmp(temp, ((t_command *)(list)->next->content)->argv[0], ft_strlen(((t_command *)(list)->next->content)->argv[0]) + 1) == 0)
					exit(0);
				dup2(pip.fd[*i][1], STDOUT_FILENO);
				printf("%s\n", temp);
				free(temp);
				dup2(out_fd, STDOUT_FILENO);
			}
		}
		wait(&pip.status);
		pip.fd[*i][1] = open("temp", O_RDONLY, 0644);
		dup2(pip.fd[*i][1], STDIN_FILENO);
		close(pip.fd[*i][1]);
		if (((t_command *)(list)->next->content)->next_flag == 3 || ((t_command *)(list)->next->content)->next_flag == 4) // (~) << (~) > or >>
		{
			exe = list->content;
			list = (list)->next;
			(*i)++;
			if_flag_right_no_exe(list, pip.fd, *i);
			to_execve_2(exe);
		}
		// printf("argv[%s]\n", ((t_command *)(list)->content)->argv[0]);
		to_execve_2(list->content);
	}
}

void	while_3456(t_list **list, int *i) //  > >> <
{
	if (((t_command *)(*list)->content)->next_flag == 6)
		unlink("temp");
	while (((t_command *)(*list)->content)->next_flag == 3 || ((t_command *)(*list)->content)->next_flag == 4) // > or >>
	{
		if (((t_command *)(*list)->next->content)->next_flag != 3 && ((t_command *)(*list)->next->content)->next_flag != 4) // aa > bb > cc
			break ;
		*list = (*list)->next;
	}
	while (((t_command *)(*list)->content)->next_flag == 5 || ((t_command *)(*list)->content)->next_flag == 6) // (~) <
	{
		if (((t_command *)(*list)->next->content)->next_flag == 3 || ((t_command *)(*list)->next->content)->next_flag == 4) // next > or >>
		{
			*list = (*list)->next;
			(*i)++;
			while (((t_command *)(*list)->content)->next_flag == 3 || ((t_command *)(*list)->content)->next_flag == 4) // >
			{
				if (((t_command *)(*list)->next->content)->next_flag != 3 && ((t_command *)(*list)->next->content)->next_flag != 4) // aa > bb > cc
					break ;
				*list = (*list)->next;
			}
		}
		if (((t_command *)(*list)->next->content)->next_flag != 5) // aa > bb > cc
			break ;
		*list = (*list)->next;
	}
}

void	do_son(t_list *list, t_pipe pip)
{
	pip.exe = list->content;
	if (((t_command *)(list)->content)->pre_flag) // (|) argv
		dup2(pip.fd[pip.i - 1][0], STDIN_FILENO);
	if_pipe(list, pip.fd, pip.i);
	if_flag_right(list, pip.exe, pip.fd, pip.i); // flag 3: >, 4: >>
	if_flag_left(list, pip.exe, pip.fd, &(pip.i)); // flag 5: <
	if_flag_d_left(list, pip.exe, pip, &(pip.i)); // flag 6: <<
	if (((t_command *)(list)->content)->next_flag == 3 || ((t_command *)(list)->content)->next_flag == 4)
		exit(0);
	to_execve_2(list->content); // 만약 다음 플레그가 > 라면 그냥 나가야함
}

void	set_pip(t_pipe *pip, t_list *list)
{
	pip->exe = NULL; //
	pip->size = ft_lstsize(list);
	pip->i = -1;
	pip->fd = malloc((sizeof(int *) * pip->size + 1));
	while (++(pip->i) < pip->size)
	{
		pip->fd[pip->i] = malloc(sizeof(int) * 2);
		pipe(pip->fd[pip->i]);
	}
	pip->i = -1;
}

t_list	*ft_pipe(t_list *list)
{
	t_pipe	pip;

	set_pip(&pip, list);
	while (++(pip.i) < pip.size && ((t_command *)(list)->content)) //&& ((t_command *)(list)->content)->next_flag) // && (*list)->content
	{
		pip.pid = fork();
		if (pip.pid == 0)
		{
			do_son(list, pip);
		}
		else // pid == 1
		{
			wait(&pip.status);
			g_data.status = WEXITSTATUS(g_data.status);
			if (((t_command *)(list)->content)->pre_flag)
				close(pip.fd[pip.i - 1][0]); // for pipe
			if (((t_command *)(list)->content)->next_flag) ////////// close
				close(pip.fd[pip.i][1]);
			while_3456(&list, &(pip.i));
		}
		if ((list)->next)
			list = (list)->next;
		else if (((t_command *)(list)->content)->next_flag == 0)
			return (list);
	}
	return (list);
}

int	minishell(char *input)
{
	t_list		*list;
	t_command	*cmd;

	input = escape_sequence(input);
	add_history(input);
	input = remove_enclosed_quotes(input);
	input_split(&g_data.commands, ft_strtrim(input, " "));
	free(input);
	set_flag(g_data.commands);
	list = g_data.commands;
	while (list != NULL)
	{
		cmd = list->content;
		if (cmd->next_flag) // |
		{
			list = ft_pipe(list);
			break ;
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
