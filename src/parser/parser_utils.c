/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:20 by mwei              #+#    #+#             */
/*   Updated: 2026/08/20 17:15:20 by mwei             ###   ########.fr       */
/*                               /                                             */
/* ************************************************************************** */

#include "cub3d.h"

int	is_empty_line(char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && (line[i] < 9 || line[i] > 13))
			return (0);
		i++;
	}
	return (1);
}

char	*skip_spaces(char *str)
{
	if (!str)
		return (NULL);
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	return (str);
}

void	consume_file(int fd)
{
	char	*line;

	if (fd < 0)
		return ;
	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
}
