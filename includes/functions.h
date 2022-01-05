/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsikim <minsikim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 21:52:01 by bahn              #+#    #+#             */
/*   Updated: 2022/01/05 09:52:38 by minsikim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "structures.h"

// void	prompt(void);
char	*prompt(void);

// Key Interrupt Handler
void	signal_handler(int signo);

// Arguments Vector Utils
int		argv_counter(char *argv[]);

// Start to Minishell
int		minishell(char *input);

// Parsing for Input
void	parsing(t_command *command);

// Run execve(path, argv, envv);
int		to_execve(t_command *command);
int		to_execve_2(t_command *command);
int		to_execve_3(t_command *command);

// COMMAND echo
int		minishell_echo(t_command *command);
int		minishell_echo_for_execve(t_command *command);

// COMMAND cd
int		minishell_cd(t_command *command);

// COMMAND pwd
int		minishell_pwd(t_command *command);

// COMMAND export
int		minishell_export(t_command *command);

// COMMAND unset
int		minishell_unset(t_command *command);

// COMMAND env
int		minishell_env(t_command *command);

// COMMAND $?
int		minishell_exit_status(t_command *command);

// Environment Variable Utils
void	set_envvpath(void);
t_hash	*get_envv(char *key);
char	*get_envv_value(char *key);
t_list	**set_lstenvv(char *env[]);
char	*envv_name_format_checker(char *key);

// Redirection Utils
void	set_redirection(void);

// Free Memory
void	minishell_finalize(void);
void	split_free(char **split);
void	command_free(void *command);

// Other Utils
char	*execfile_finder(char *command);
size_t	redirection_finder(char *input, char *redirection[], char **save);

void	ft_split_command(t_list **list, char *s, char **redirect);
void	input_split(t_list **list, char *input);

char	*remove_enclosed_quotes(char *arg);
char	*escape_sequence(char *arg);

int		incorrect_exit(t_command *command);

#endif