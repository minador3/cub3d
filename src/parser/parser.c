/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:16:10 by mwei              #+#    #+#             */
/*   Updated: 2026/08/20 17:16:10 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_parser_vars(t_game *game)
{
	game->tex.no_path = NULL;
	game->tex.so_path = NULL;
	game->tex.we_path = NULL;
	game->tex.ea_path = NULL;
	game->map.grid = NULL;
	game->map.w = 0;
	game->map.h = 0;
	game->map.has_floor = 0;
	game->map.has_ceil = 0;
}

static int	check_extension(char *path, char *ext)
{
	int	len;
	int	ext_len;

	len = ft_strlen(path);
	ext_len = ft_strlen(ext);
	if (len < ext_len)
		return (0);
	return (ft_strncmp(path + len - ext_len, ext, ext_len) == 0);
}

void	parse_cub_file(t_game *game, char *path)
{
	int		fd;
	char	*line;
	int		res;

	if (!check_extension(path, ".cub"))
		error_exit("File extension must be .cub");
	init_parser_vars(game);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_exit("Failed to open file.");
	line = get_next_line(fd);
	while (line && !is_config_complete(game))
	{
		res = parse_config_line(game, line);
		if (res == -1)
		{
			free(line);
			consume_file(fd);
			close(fd);
			error_exit_game(game, "Invalid configuration element in file.");
		}
		free(line);
		line = get_next_line(fd);
	}
	if (!is_config_complete(game))
	{
		consume_file(fd);
		close(fd);
		error_exit_game(game, "Incomplete configuration in file.");
	}
	while (line && is_empty_line(line))
	{
		free(line);
		line = get_next_line(fd);
	}
	if (!line)
	{
		consume_file(fd);
		close(fd);
		error_exit_game(game, "No map found in file.");
	}
	read_map_grid(game, fd, line);
	close(fd);
	validate_map_characters(game);
	check_map_enclosure(game);
}
