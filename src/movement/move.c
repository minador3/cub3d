#include "cub3d.h"

static int	is_walkable(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_x >= game->map.w
		|| map_y < 0 || map_y >= game->map.h)
		return (0);
	if (game->map.grid[map_y][map_x] == '1'
		|| game->map.grid[map_y][map_x] == ' ')
		return (0);
	return (1);
}

static int	can_move_to(t_game *game, double new_x, double new_y)
{
	double	margin;

	margin = 0.2;
	if (!is_walkable(game, new_x - margin, new_y - margin))
		return (0);
	if (!is_walkable(game, new_x + margin, new_y - margin))
		return (0);
	if (!is_walkable(game, new_x - margin, new_y + margin))
		return (0);
	if (!is_walkable(game, new_x + margin, new_y + margin))
		return (0);
	return (1);
}

void	move_forward(t_game *game, double speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x + game->player.dir_x * speed;
	new_y = game->player.y + game->player.dir_y * speed;
	if (can_move_to(game, new_x, game->player.y))
		game->player.x = new_x;
	if (can_move_to(game, game->player.x, new_y))
		game->player.y = new_y;
}

void	move_backward(t_game *game, double speed)
{
	move_forward(game, -speed);
}

void	strafe_right(t_game *game, double speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x - game->player.dir_y * speed;
	new_y = game->player.y + game->player.dir_x * speed;
	if (can_move_to(game, new_x, game->player.y))
		game->player.x = new_x;
	if (can_move_to(game, game->player.x, new_y))
		game->player.y = new_y;
}
