/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:50 by mwei              #+#    #+#             */
/*   Updated: 2026/08/25 14:10:10 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_player_dir(t_game *game, char dir)
{
	game->player.spawn_dir = dir;
	if (dir == 'N')
	{
		game->player.dir_x = 0.0;
		game->player.dir_y = -1.0;
		game->player.plane_x = 0.66;
		game->player.plane_y = 0.0;
	}
	else if (dir == 'S')
	{
		game->player.dir_x = 0.0;
		game->player.dir_y = 1.0;
		game->player.plane_x = -0.66;
		game->player.plane_y = 0.0;
	}
	else if (dir == 'E')
	{
		game->player.dir_x = 1.0;
		game->player.dir_y = 0.0;
		game->player.plane_x = 0.0;
		game->player.plane_y = 0.66;
	}
	else if (dir == 'W')
	{
		game->player.dir_x = -1.0;
		game->player.dir_y = 0.0;
		game->player.plane_x = 0.0;
		game->player.plane_y = -0.66;
	}
}

void	validate_map_characters(t_game *game)
{
	int	x;
	int	y;
	int	p_count;

	p_count = 0;
	y = 0;
	while (y < game->map.h)
	{
		x = 0;
		while (x < game->map.w)
		{
			if (game->map.grid[y][x] != '1' && game->map.grid[y][x] != '0'
				&& game->map.grid[y][x] != ' ' && game->map.grid[y][x] != 'N'
				&& game->map.grid[y][x] != 'S' && game->map.grid[y][x] != 'E'
				&& game->map.grid[y][x] != 'W')
				error_exit_game(game, "Map contains invalid characters.");
			if (game->map.grid[y][x] == 'N' || game->map.grid[y][x] == 'S'
				|| game->map.grid[y][x] == 'E' || game->map.grid[y][x] == 'W')
			{
				p_count++;
				game->player.x = x + 0.5;
				game->player.y = y + 0.5;
				set_player_dir(game, game->map.grid[y][x]);
			}
			x++;
		}
		y++;
	}
	if (p_count == 0)
		error_exit_game(game, "Map must have exactly one player starting position.");
	if (p_count > 1)
		error_exit_game(game, "Map must have exactly one player starting position.");
}
