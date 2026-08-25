/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:40 by mwei              #+#    #+#             */
/*   Updated: 2026/08/25 14:10:21 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_line_blank(char *line)
{
	int	i;

	if (!line || line[0] == '\0')
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] != '\n' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

static void	strip_newline(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\r')
		line[len - 1] = '\0';
}

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
	game->map.grid = malloc(sizeof(char *) * (game->map.h + 1));
	if (!game->map.grid)
	{
		ft_lstclear(&map_list, free);
		error_exit_game(game, "Memory allocation failed for map grid.");
	}
	curr = map_list;
	i = 0;
	while (curr)
	{
		game->map.grid[i] = pad_line((char *)curr->content, game->map.w);
		if (!game->map.grid[i])
		{
			ft_lstclear(&map_list, free);
			error_exit_game(game, "Memory allocation failed during map padding.");
		}
		curr = curr->next;
		i++;
	}
	game->map.grid[i] = NULL;
}

void	read_map_grid(t_game *game, int fd, char *first_line)
{
	t_list	*map_list;
	t_list	*new_node;
	char	*line;
	int		map_ended;

	map_list = NULL;
	map_ended = 0;
	line = first_line;
	while (line)
	{
		if (is_line_blank(line))
		{
			if (map_list != NULL)
				map_ended = 1;
			free(line);
		}
		else
		{
			if (map_ended)
			{
				ft_lstclear(&map_list, free);
				free(line);
				consume_file(fd);
				close(fd);
				error_exit_game(game, "Map contains empty line.");
			}
			strip_newline(line);
			if ((int)ft_strlen(line) > game->map.w)
				game->map.w = ft_strlen(line);
			new_node = ft_lstnew(line);
			if (!new_node)
			{
				free(line);
				ft_lstclear(&map_list, free);
				consume_file(fd);
				close(fd);
				error_exit_game(game, "Memory allocation failed for map list.");
			}
			ft_lstadd_back(&map_list, new_node);
		}
		line = get_next_line(fd);
	}
	convert_list_to_grid(game, map_list);
	ft_lstclear(&map_list, free);
}
