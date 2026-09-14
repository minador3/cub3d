/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:40 by mwei              #+#    #+#             */
/*   Updated: 2026/09/14 16:30:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*pad_line(char *line, int width)
{
	char	*padded;
	int		len;
	int		i;

	padded = malloc(sizeof(char) * (width + 1));
	if (!padded)
		return (NULL);
	len = ft_strlen(line);
	i = 0;
	while (i < len)
	{
		padded[i] = line[i];
		i++;
	}
	while (i < width)
	{
		padded[i] = ' ';
		i++;
	}
	padded[width] = '\0';
	return (padded);
}

static void	convert_list_to_grid(t_game *game, t_list *map_list)
{
	t_list	*curr;
	int		i;

	game->map.h = ft_lstsize(map_list);
	if (game->map.h == 0)
		error_exit_game(game, "Map is empty.");
	game->map.grid = ft_calloc(game->map.h + 1, sizeof(char *));
	if (!game->map.grid)
	{
		ft_lstclear(&map_list, free);
		error_exit_game(game, "Map grid allocation failed.");
	}
	curr = map_list;
	i = 0;
	while (curr)
	{
		game->map.grid[i] = pad_line((char *)curr->content, game->map.w);
		if (!game->map.grid[i++])
		{
			ft_lstclear(&map_list, free);
			error_exit_game(game, "Map padding failed.");
		}
		curr = curr->next;
	}
}

static void	append_map_line(t_game *game, t_list **list, char *line, int fd)
{
	int		len;
	t_list	*new_node;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[--len] = '\0';
	if (len > 0 && line[len - 1] == '\r')
		line[--len] = '\0';
	if (len > game->map.w)
		game->map.w = len;
	new_node = ft_lstnew(line);
	if (!new_node)
	{
		free(line);
		ft_lstclear(list, free);
		consume_file(fd);
		close(fd);
		error_exit_game(game, "Memory allocation failed for map list.");
	}
	ft_lstadd_back(list, new_node);
}

static void	map_line_err(t_game *game, t_list *map, char *line, int fd)
{
	ft_lstclear(&map, free);
	free(line);
	consume_file(fd);
	close(fd);
	error_exit_game(game, "Map contains empty line.");
}

void	read_map_grid(t_game *game, int fd, char *first_line)
{
	t_list	*map_list;
	char	*line;
	int		map_ended;

	map_list = NULL;
	map_ended = 0;
	line = first_line;
	while (line)
	{
		if (is_empty_line(line))
		{
			if (map_list != NULL)
				map_ended = 1;
			free(line);
		}
		else if (map_ended)
			map_line_err(game, map_list, line, fd);
		else
			append_map_line(game, &map_list, line, fd);
		line = get_next_line(fd);
	}
	convert_list_to_grid(game, map_list);
	ft_lstclear(&map_list, free);
}
