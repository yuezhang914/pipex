/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:32:39 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/17 00:41:03 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		tube[2];
	pid_t	pid1;
	pid_t	pid2;
	char	*paths;
	char	**cmd_paths;
	char	**cmd_args;
	char	*cmd;
	char	*progname;
}			t_pipex;

void		init_pipex(t_pipex *p, char *progname);
char		*get_cmd(char **paths, char *cmd);
void		first_child(t_pipex pipex, char *argv[], char *envp[]);
void		second_child(t_pipex pipex, char *argv[], char *envp[]);
int			msg(t_pipex *p, const char *err);
void		msg_error(t_pipex *p, const char *target);
void		parent_free(t_pipex *pipex);
void		child_free(t_pipex *pipex);
char		*find_path(char **envp);
void		close_pipes(t_pipex *pipex);

#endif
