/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:16:20 by mwei              #+#    #+#             */
/*   Updated: 2026/09/14 16:30:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <unistd.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3D <path_to_map.cub>\n", 2);
		return (1);
	}
	ft_memset(&game, 0, sizeof(t_game));
	parse_cub_file(&game, argv[1]);
	init_window(&game);
	load_all_textures(&game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.win, 3, 1L << 1, handle_keyrelease, &game);
	mlx_hook(game.win, 17, 1L << 17, close_game, &game);
	mlx_loop(game.mlx);
	return (0);
}
