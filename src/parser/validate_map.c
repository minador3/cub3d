/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:50 by mwei              #+#    #+#             */
/*   Updated: 2026/09/14 16:30:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_dir_ns(t_game *game, char dir)
{
	if (dir == 'N')
	{
		game->player.dir_y = -1.0;
		game->player.plane_x = 0.66;
	}
	else if (dir == 'S')
	{
		game->player.dir_y = 1.0;
		game->player.plane_x = -0.66;
	}
}

static void	set_dir_ew(t_game *game, char dir)
{
	if (dir == 'E')
	{
		game->player.dir_x = 1.0;
		game->player.plane_y = 0.66;
	}
	else if (dir == 'W')
	{
		game->player.dir_x = -1.0;
		game->player.plane_y = -0.66;
	}
}

static void	set_player_dir(t_game *game, char dir)
{
	game->player.spawn_dir = dir;
	game->player.dir_x = 0.0;
	game->player.dir_y = 0.0;
	game->player.plane_x = 0.0;
	game->player.plane_y = 0.0;
	if (dir == 'N' || dir == 'S')
		set_dir_ns(game, dir);
	else if (dir == 'E' || dir == 'W')
		set_dir_ew(game, dir);
}

static void	record_player(t_game *game, int x, int y, int *p_count)
{
	(*p_count)++;
	game->player.x = x + 0.5;
	game->player.y = y + 0.5;
	set_player_dir(game, game->map.grid[y][x]);
	game->map.grid[y][x] = '0';
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
			if (!ft_strchr("01 NSEW", game->map.grid[y][x]))
				error_exit_game(game, "Map contains invalid characters.");
			if (ft_strchr("NSEW", game->map.grid[y][x]))
				record_player(game, x, y, &p_count);
			x++;
		}
		y++;
	}
	if (p_count != 1)
		error_exit_game(game, "Map must have one player start position.");
}
