/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:16:20 by mwei              #+#    #+#             */
/*   Updated: 2026/08/20 17:16:20 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	printf("Map parsed and validated successfully!\n");
	printf("Map size: %d x %d\n", game.map.w, game.map.h);
	printf("Player position: (%.2f, %.2f) facing '%c'\n",
		game.player.x, game.player.y, game.player.spawn_dir);
	printf("Ceiling color: #%06X, Floor color: #%06X\n",
		game.map.ceil_hex, game.map.floor_hex);
	cleanup_game(&game);
	return (0);
}
