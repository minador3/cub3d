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
	exit(0);
}

void	error_exit(char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}

static void	cleanup_textures(t_game *game)
{
	if (game->tex.no_img.ptr)
		mlx_destroy_image(game->mlx, game->tex.no_img.ptr);
	if (game->tex.so_img.ptr)
		mlx_destroy_image(game->mlx, game->tex.so_img.ptr);
	if (game->tex.we_img.ptr)
		mlx_destroy_image(game->mlx, game->tex.we_img.ptr);
	if (game->tex.ea_img.ptr)
		mlx_destroy_image(game->mlx, game->tex.ea_img.ptr);
	if (game->screen.ptr)
		mlx_destroy_image(game->mlx, game->screen.ptr);
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	if (game->map.grid)
		free_map(game->map.grid);
	game->map.grid = NULL;
	free(game->tex.no_path);
	game->tex.no_path = NULL;
	free(game->tex.so_path);
	game->tex.so_path = NULL;
	free(game->tex.we_path);
	game->tex.we_path = NULL;
	free(game->tex.ea_path);
	game->tex.ea_path = NULL;
	if (game->mlx)
	{
		cleanup_textures(game);
		if (game->win)
			mlx_destroy_window(game->mlx, game->win);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}

void	error_exit_game(t_game *game, char *msg)
{
	cleanup_game(game);
	error_exit(msg);
}
