# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bahn <bahn@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/18 15:05:30 by bahn              #+#    #+#              #
#    Updated: 2022/01/18 00:15:09 by bahn             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OS	= $(shell uname -s)

ifeq ($(OS), Darwin)
export LDFLAGS := -L/opt/homebrew/opt/readline/lib
export CPPFLAGS := -I/opt/homebrew/opt/readline/include
endif

NAME = minishell

CC = gcc
CFLAGS = -Wall -Werror -Wextra #-g
INCFLAGS = -I./includes -I./libft $(CPPFLAGS)

RM = rm -fv

LIBFT_PATH = ./libft
LIBFT_LINK = -Llibft -lft

SRCS_PATH = ./srcs/
SRCS_NAME = main.c minishell.c
SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))
OBJS = $(SRCS:.c=.o)

PROMPT_SRCS_PATH = ./srcs/prompt/
PROMPT_SRCS_NAME = prompt.c
PROMPT_SRCS = $(addprefix $(PROMPT_SRCS_PATH), $(PROMPT_SRCS_NAME))
PROMPT_OBJS = $(PROMPT_SRCS:.c=.o)

SIG_SRCS_PATH = ./srcs/signal/
SIG_SRCS_NAME = handler.c 
SIG_SRCS = $(addprefix $(SIG_SRCS_PATH), $(SIG_SRCS_NAME))
SIG_OBJS = $(SIG_SRCS:.c=.o)

GNL_SRCS_PATH = ./srcs/gnl/
GNL_SRCS_NAME = get_next_line.c
GNL_SRCS = $(addprefix $(GNL_SRCS_PATH), $(GNL_SRCS_NAME))
GNL_OBJS = $(GNL_SRCS:.c=.o)

CMD_SRCS_PATH = ./srcs/command/
CMD_SRCS_NAME = echo.c \
				cd.c \
				pwd.c \
				export.c \
				unset.c \
				env.c
CMD_SRCS = $(addprefix $(CMD_SRCS_PATH), $(CMD_SRCS_NAME))
CMD_OBJS = $(CMD_SRCS:.c=.o)

PARSE_SRCS_PATH = ./srcs/parse/
PARSE_SRCS_NAME = execution.c \
					more_input.c \
					arg_finder.c \
					input_split.c \
					abbreviation_converter.c \
					escape_sequence.c \
					backslash.c
PARSE_SRCS = $(addprefix $(PARSE_SRCS_PATH), $(PARSE_SRCS_NAME))
PARSE_OBJS = $(PARSE_SRCS:.c=.o)

ENVV_SRCS_PATH = ./srcs/envv/
ENVV_SRCS_NAME = set_envv.c \
					get_envv.c \
					envv_name_format_checker.c \
					envv_converter.c
ENVV_SRCS = $(addprefix $(ENVV_SRCS_PATH), $(ENVV_SRCS_NAME))
ENVV_OBJS = $(ENVV_SRCS:.c=.o)

EXEC_SRCS_PATH = ./srcs/execve/
EXEC_SRCS_NAME = to_execve.c \
					envp_to_arr_converter.c
EXEC_SRCS = $(addprefix $(EXEC_SRCS_PATH), $(EXEC_SRCS_NAME))
EXEC_OBJS = $(EXEC_SRCS:.c=.o)

UTILS_SRCS_PATH = ./srcs/utils/
UTILS_SRCS_NAME = execfile_finder.c \
					argv_counter.c \
					add_arguments.c \
					incorrect_exit.c \
					ft_strjoin_with_free.c \
					set_redirection.c \
					redirection_finder.c
UTILS_SRCS = $(addprefix $(UTILS_SRCS_PATH), $(UTILS_SRCS_NAME))
UTILS_OBJS = $(UTILS_SRCS:.c=.o)

FREE_SRCS_PATH = ./srcs/free/
FREE_SRCS_NAME = hash_free.c \
					split_free.c \
					command_free.c
FREE_SRCS = $(addprefix $(FREE_SRCS_PATH), $(FREE_SRCS_NAME))
FREE_OBJS = $(FREE_SRCS:.c=.o)

BAHN_SRCS_PATH = ./srcs/bahn_redir/
BAHN_SRCS_NAME = redirection.c \
					pipe.c
BAHN_SRCS = $(addprefix $(BAHN_SRCS_PATH), $(BAHN_SRCS_NAME))
BAHN_OBJS = $(BAHN_SRCS:.c=.o)

ALL_OBJS = $(OBJS) \
			$(SIG_OBJS) \
			$(PROMPT_OBJS) \
			$(GNL_OBJS) \
			$(CMD_OBJS) \
			$(PARSE_OBJS) \
			$(ENVV_OBJS) \
			$(EXEC_OBJS) \
			$(UTILS_OBJS) \
			$(FREE_OBJS) \
			$(BAHN_OBJS)

all : $(NAME)

.c.o :
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(NAME) : $(ALL_OBJS)
		git submodule update --init --recursive
		$(MAKE) all bonus -C $(LIBFT_PATH)
		$(CC) $(CFLAGS) $(INCFLAGS) $^ $(LIBFT_LINK) $(LDFLAGS) -lreadline -o $@

clean : 
		$(MAKE) clean -C $(LIBFT_PATH)
		$(RM) $(ALL_OBJS)

fclean : clean
		$(MAKE) fclean -C $(LIBFT_PATH)
		$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re