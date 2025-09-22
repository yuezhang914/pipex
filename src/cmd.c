/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 02:08:53 by yzhang2           #+#    #+#             */
/*   Updated: 2025/09/20 05:22:35 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*strjoin_plus(const char *a, const char *b, const char *c)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(a, b);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, c);
	free(tmp);
	return (result);
}

static char	*try_direct(char *cmd)
{
	if (!cmd)
		return (NULL);
	if (!ft_strchr(cmd, '/'))
		return (NULL);
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*build_candidate(const char *path, const char *cmd)
{
	if (!cmd)
		return (NULL);
	if (!path || path[0] == '\0')
		return (ft_strdup(cmd));
	return (strjoin_plus(path, "/", cmd));
}

char	*get_cmd(char **paths, char *cmd)
{
	char	*candidate;
	char	**p;
	char	*direct;

	if (!cmd)
		return (NULL);
	direct = try_direct(cmd);
	if (direct)
		return (direct);
	p = paths;
	while (p && *p)
	{
		candidate = build_candidate(*p, cmd);
		if (!candidate)
			return (NULL);
		if (access(candidate, F_OK) == 0)
			return (candidate);
		free(candidate);
		p++;
	}
	return (NULL);
}
