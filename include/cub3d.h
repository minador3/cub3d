/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:00 by mwei              #+#    #+#             */
/*   Updated: 2026/09/14 16:30:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../minilibx-linux/mlx.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>

# define SUCCESS 0
# define FAILURE 1
# define MOVE_SPEED 0.05
# define ROTATE_SPEED 0.03

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

typedef struct s_ray
{
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
	int		side;
}			t_ray;

typedef struct s_wall
{
	double	perp_wall_dist;
	int		line_height;
	int		draw_start;
	int		draw_end;
	double	wall_x;
	int		tex_x;
}			t_wall;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}			t_keys;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_map		map;
	t_tex		tex;
	t_player	player;
	t_img		screen;
	t_keys		keys;
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
int		parse_texture(char *s, char **path_dst);

void	init_dda(t_player *player, t_ray *ray);
void	dda_loop(t_ray *ray, t_game *game);
void	calc_wall_height(t_ray *ray, t_wall *wall, int screen_height);

void	init_window(t_game *game);
void	my_mlx_pixel_put(t_img *img, int x, int y, int colour);
void	paint_ceiling_floor(t_game *game);
void	render_walls(t_game *game);
void	paint_wall_column(t_game *game, int x, t_ray *ray, t_wall *wall);
t_img	*select_texture(t_ray *ray, t_tex *tex);

void	load_one_texture(t_game *game, char *path, t_img *tex_img);
void	load_all_textures(t_game *game);
int		game_loop(t_game *game);

int		close_game(t_game *game);
void	move_forward(t_game *game, double speed);
void	move_backward(t_game *game, double speed);
void	strafe_right(t_game *game, double speed);
void	strafe_left(t_game *game, double speed);
void	rotate_player(t_game *game, double theta);

int		handle_keypress(int keycode, t_game *game);
int		handle_keyrelease(int keycode, t_game *game);
void	process_movement(t_game *game);

#endif
