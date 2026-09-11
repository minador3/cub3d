/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:16:00 by mwei              #+#    #+#             */
/*   Updated: 2026/08/20 17:16:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	**duplicate_map(t_game *game)
{
	char	**temp;
	int		y;

	temp = malloc(sizeof(char *) * (game->map.h + 1));
	if (!temp)
		error_exit_game(game, "Memory allocation failed in flood fill.");
	y = 0;
	while (y < game->map.h)
	{
		temp[y] = ft_strdup(game->map.grid[y]);
		if (!temp[y])
		{
			free_map(temp);
			error_exit_game(game, "Memory allocation failed in flood fill.");
		}
		y++;
	}
	temp[y] = NULL;
	return (temp);
}

static void	flood_check(char **grid, int x, int y, int height, int width, int *leak)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		*leak = 1;
		return ;
	}
	if (grid[y][x] == ' ')
	{
		*leak = 1;
		return ;
	}
	if (grid[y][x] == '1' || grid[y][x] == 'V')
		return ;
	grid[y][x] = 'V';
	flood_check(grid, x + 1, y, height, width, leak);
	flood_check(grid, x - 1, y, height, width, leak);
	flood_check(grid, x, y + 1, height, width, leak);
	flood_check(grid, x, y - 1, height, width, leak);
}

void	check_map_enclosure(t_game *game)
{
	char	**temp;
	int		x;
	int		y;
	int		leak;

	temp = duplicate_map(game);
	leak = 0;
	y = 0;
	while (y < game->map.h)
	{
		x = 0;
		while (x < game->map.w)
		{
			if (temp[y][x] == '0' || temp[y][x] == 'N' || temp[y][x] == 'S'
				|| temp[y][x] == 'E' || temp[y][x] == 'W')
			{
				flood_check(temp, x, y, game->map.h, game->map.w, &leak);
				if (leak)
				{
					free_map(temp);
					error_exit_game(game, "Map is not enclosed by walls.");
				}
			}
			x++;
		}
		y++;
	}
	free_map(temp);
}
