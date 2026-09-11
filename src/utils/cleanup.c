/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:10 by mwei              #+#    #+#             */
/*   Updated: 2026/08/20 17:15:10 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	close_game(t_game *game)
{
	cleanup_game(game);
	mlx_destroy_image(game->mlx, game->screen.ptr);
	mlx_destroy_window(game->mlx, game->win);
	exit(0);
}

void	error_exit(char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}

void	free_map(char **map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	if (game->map.grid)
	{
		free_map(game->map.grid);
		game->map.grid = NULL;
	}
	if (game->tex.no_path)
	{
		free(game->tex.no_path);
		game->tex.no_path = NULL;
	}
	if (game->tex.so_path)
	{
		free(game->tex.so_path);
		game->tex.so_path = NULL;
	}
	if (game->tex.we_path)
	{
		free(game->tex.we_path);
		game->tex.we_path = NULL;
	}
	if (game->tex.ea_path)
	{
		free(game->tex.ea_path);
		game->tex.ea_path = NULL;
	}
	// Note: Teammate B will add MLX image and window destroying logic here.
}

void	error_exit_game(t_game *game, char *msg)
{
	cleanup_game(game);
	error_exit(msg);
}
