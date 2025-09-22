/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 01:43:43 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/20 02:20:36 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define ERR_INPUT "Error: invalid arguments\n"
# define ERR_PIPE "Error: pipe\n"
# define ERR_INFILE "Error: infile\n"
# define ERR_OUTFILE "Error: outfile\n"
# define ERR_CMD "command not found: "
# define ERR_ENVP "Error: PATH not found\n"

typedef struct s_ppxb
{
	int		infile;
	int		outfile;
	int		*pipe;
	int		cmd_nmbs;
	int		pipe_nmbs;
	int		here_doc;
	char	*env_path;
	char	**cmd_paths;
	char	**cmd_args;
	char	*cmd;
	int		idx;
	pid_t	*pids;
}			t_ppxb;

/* main */
void		init_pipex(t_ppxb *p);

/* parse */
int			args_in(int argc, char **argv, t_ppxb *pipex);
void		get_infile(int argc, char **argv, t_ppxb *pipex);
void		get_outfile(char *argv_last, t_ppxb *pipex);
char		*find_path(char **envp);

/* pipes */
int			*alloc_pipes(int pipe_nmbs);
void		create_pipes(t_ppxb *pipex);
void		close_pipes(t_ppxb *pipex);
void		free_pipes(t_ppxb *pipex);

/* here_doc */
int			do_here_doc(char *limiter);

/* exec */
void		exec_all_cmds(t_ppxb *pipex, char **argv, char **envp);
void		exec_child(t_ppxb *pipex, char **argv, char **envp);

/* cleanup */
int			wait_and_cleanup(t_ppxb *pipex);
void		parent_free(t_ppxb *pipex);
void		child_free(t_ppxb *pipex);

/* utils */
char		*get_cmd(char **paths, char *cmd);
void		msg_pipe(char *arg);
int			msg(t_ppxb *pipex, const char *err);
void		msg_error(t_ppxb *pipex, const char *target);

#endif
