/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:00 by mwei              #+#    #+#             */
/*   Updated: 2026/08/20 17:15:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>

# define SUCCESS 0
# define FAILURE 1

typedef struct s_img
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		w;
	int		h;
}			t_img;

typedef struct s_tex
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	t_img	no_img;
	t_img	so_img;
	t_img	we_img;
	t_img	ea_img;
}			t_tex;

typedef struct s_map
{
	char	**grid;
	int		w;
	int		h;
	int		floor_color[3];
	int		ceil_color[3];
	int		floor_hex;
	int		ceil_hex;
	int		has_floor;
	int		has_ceil;
}			t_map;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	char	spawn_dir;
}			t_player;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_map		map;
	t_tex		tex;
	t_player	player;
	t_img		screen;
}				t_game;

/* Cleanup and Errors */
void	error_exit(char *msg);
void	error_exit_game(t_game *game, char *msg);
void	free_map(char **map);
void	cleanup_game(t_game *game);

/* Parsing entry */
void	parse_cub_file(t_game *game, char *path);
int		parse_config_line(t_game *game, char *line);
int		is_config_complete(t_game *game);
void	read_map_grid(t_game *game, int fd, char *first_line);
void	validate_map_characters(t_game *game);
void	check_map_enclosure(t_game *game);

/* Parsing utilities */
int		is_empty_line(char *line);
char	*skip_spaces(char *str);
void	consume_file(int fd);

#endif
