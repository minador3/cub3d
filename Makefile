NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude

LIBFT_DIR = libft
MLX_DIR = minilibx-linux

LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx.a

SRC = src/main.c \
      src/parser/parser.c \
      src/parser/parse_config.c \
      src/parser/parse_map.c \
      src/parser/parser_utils.c \
      src/parser/validate_map.c \
      src/parser/flood_fill.c \
      src/utils/cleanup.c	\
	  src/raycast/vectors.c \
	  src/render/init_mlx.c	\
	  src/render/pixel_put.c \
	  src/render/load_textures.c \
	  src/movement/keypress_move.c \
	  src/movement/move.c

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

LFLAGS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

all: $(NAME)

$(LIBFT):
	@make bonus -C $(LIBFT_DIR)

$(MLX):
	@make -C $(MLX_DIR)

$(NAME): $(LIBFT) $(MLX) $(OBJ)
	@$(CC) -no-pie $(CFLAGS) $(OBJ) $(LFLAGS) -o $(NAME)
	@echo "Done Making $(NAME)"

$(OBJ_DIR)/%.o: %.c include/cub3d.h
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(MLX_DIR)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
