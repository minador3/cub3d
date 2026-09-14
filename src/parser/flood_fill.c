/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:16:00 by mwei              #+#    #+#             */
/*   Updated: 2026/09/14 16:30:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	**duplicate_map(t_game *game)
{
	char	**temp;
	int		y;

	temp = ft_calloc(game->map.h + 1, sizeof(char *));
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

static int	flood_check(t_game *game, char **grid, int x, int y)
{
	if (x < 0 || x >= game->map.w || y < 0 || y >= game->map.h)
		return (1);
	if (grid[y][x] == ' ')
		return (1);
	if (grid[y][x] == '1' || grid[y][x] == 'V')
		return (0);
	grid[y][x] = 'V';
	if (flood_check(game, grid, x + 1, y))
		return (1);
	if (flood_check(game, grid, x - 1, y))
		return (1);
	if (flood_check(game, grid, x, y + 1))
		return (1);
	if (flood_check(game, grid, x, y - 1))
		return (1);
	return (0);
}

void	check_map_enclosure(t_game *game)
{
	char	**temp;
	int		x;
	int		y;

	temp = duplicate_map(game);
	y = 0;
	while (y < game->map.h)
	{
		x = 0;
		while (x < game->map.w)
		{
			if (temp[y][x] == '0' && flood_check(game, temp, x, y))
			{
				free_map(temp);
				error_exit_game(game, "Map is not enclosed by walls.");
			}
			x++;
		}
		y++;
	}
	free_map(temp);
}
