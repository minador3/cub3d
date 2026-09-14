/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwei <mwei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 17:15:30 by mwei              #+#    #+#             */
/*   Updated: 2026/09/14 16:30:00 by mwei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	parse_rgb_value(char **str)
{
	int		val;
	int		count;
	char	*s;

	s = skip_spaces(*str);
	if (!ft_isdigit(*s))
		return (-1);
	val = 0;
	count = 0;
	while (ft_isdigit(*s))
	{
		val = val * 10 + (*s - '0');
		count++;
		if (count > 3 || val > 255)
			return (-1);
		s++;
	}
	s = skip_spaces(s);
	*str = s;
	return (val);
}

static int	parse_color(char *line, int color_arr[3])
{
	char	*s;
	int		i;
	int		commas;

	s = skip_spaces(line + 1);
	commas = 0;
	i = -1;
	while (s[++i])
		if (s[i] == ',')
			commas++;
	if (commas != 2)
		return (0);
	i = 0;
	while (i < 3)
	{
		color_arr[i] = parse_rgb_value(&s);
		if (color_arr[i] < 0 || color_arr[i] > 255)
			return (0);
		if (i < 2 && *s != ',')
			return (0);
		if (i < 2 && *s == ',')
			s++;
		i++;
	}
	return (*s == '\0' || *s == '\n' || *s == '\r');
}

static int	set_color(int color[3], int *hex, int *has, char *s)
{
	if (*has || !parse_color(s, color))
		return (-1);
	*has = 1;
	*hex = (color[0] << 16) | (color[1] << 8) | color[2];
	return (1);
}

static int	parse_tex_config(t_game *game, char *s)
{
	if (!ft_strncmp(s, "NO ", 3) || !ft_strncmp(s, "NO\t", 3))
		return (parse_texture(s + 2, &game->tex.no_path));
	if (!ft_strncmp(s, "SO ", 3) || !ft_strncmp(s, "SO\t", 3))
		return (parse_texture(s + 2, &game->tex.so_path));
	if (!ft_strncmp(s, "WE ", 3) || !ft_strncmp(s, "WE\t", 3))
		return (parse_texture(s + 2, &game->tex.we_path));
	if (!ft_strncmp(s, "EA ", 3) || !ft_strncmp(s, "EA\t", 3))
		return (parse_texture(s + 2, &game->tex.ea_path));
	return (0);
}

int	parse_config_line(t_game *game, char *line)
{
	char	*s;
	int		res;

	s = skip_spaces(line);
	if (*s == '\0' || *s == '\n' || *s == '\r')
		return (0);
	res = parse_tex_config(game, s);
	if (res != 0)
		return (res);
	if (!ft_strncmp(s, "F ", 2) || !ft_strncmp(s, "F\t", 2))
		return (set_color(game->map.floor_color, &game->map.floor_hex,
				&game->map.has_floor, s));
	if (!ft_strncmp(s, "C ", 2) || !ft_strncmp(s, "C\t", 2))
		return (set_color(game->map.ceil_color, &game->map.ceil_hex,
				&game->map.has_ceil, s));
	return (-1);
}
