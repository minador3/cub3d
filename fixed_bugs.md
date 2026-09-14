# cub3D: Bug Fixes and Code Improvements

This document details all the bugs identified and resolved in the **cub3D** codebase.

---

## 1. Inverted Wall Textures (All 4 Cardinal Directions)
* **File:** [`src/render/pixel_put.c`](file:///home/mwei/42Projects/Cub3D/src/render/pixel_put.c) (`select_texture`)
* **Problem:** The conditions checking the ray direction were inverted:
  * When ray was travelling North (`step_y < 0`), it selected `so_img` (South).
  * When ray was travelling South (`step_y > 0`), it selected `no_img` (North).
  * When ray was travelling East (`step_x > 0`), it selected `we_img` (West).
  * When ray was travelling West (`step_x < 0`), it selected `ea_img` (East).
  As a result, looking towards a northern wall rendered the South texture ('S'), etc.
* **Solution:** Corrected the mapping so rays hitting a wall face select the texture of that face:
  * Looking North (`side == 1 && step_y < 0`) $\rightarrow$ `NO` (`no_img`)
  * Looking South (`side == 1 && step_y > 0`) $\rightarrow$ `SO` (`so_img`)
  * Looking East (`side == 0 && step_x > 0`) $\rightarrow$ `EA` (`ea_img`)
  * Looking West (`side == 0 && step_x < 0`) $\rightarrow$ `WE` (`we_img`)

---

## 2. Horizontal Texture Mirroring & Unclamped Coordinates
* **File:** [`src/render/pixel_put.c`](file:///home/mwei/42Projects/Cub3D/src/render/pixel_put.c) (`paint_wall_column`)
* **Problem:**
  * Screen columns advance from left to right, but on South-facing and West-facing walls, the hit coordinate $wall_x$ runs in the opposite direction. Without horizontal flipping, textures on these walls were drawn mirrored (letters and patterns appeared backwards).
  * If $wall_x$ hit $1.0$ due to floating-point rounding, `(int)(wall_x * tex->w)` could equal `tex->w`, causing an out-of-bounds pixel read.
* **Solution:**
  * Flipped the texture coordinate for West and South walls:
    ```c
    if ((ray->side == 0 && ray->ray_dir_x < 0)
        || (ray->side == 1 && ray->ray_dir_y > 0))
        wall->tex_x = tex->w - wall->tex_x - 1;
    ```
  * Added boundary clamping for `wall->tex_x` to `[0, tex->w - 1]`.

---

## 3. Vertical Texture Distortion Close to Walls & Division by Zero
* **File:** [`src/render/pixel_put.c`](file:///home/mwei/42Projects/Cub3D/src/render/pixel_put.c) (`paint_wall_column`, `draw_slice`)
* **Problem:**
  * `tex_y` was previously calculated as:
    ```c
    tex_y = ((y - wall->draw_start) * tex->h) / wall->line_height;
    ```
    When the player stood close to a wall, `line_height` exceeded `screen_height`, so `draw_start` was clamped to `0`. At the top screen pixel (`y = 0`), `(y - draw_start)` was always `0`. This locked the top pixel to the top of the texture, squashing the texture vertically instead of zooming into the center.
  * If `wall->line_height == 0`, integer division by zero caused a crash.
* **Solution:**
  * Replaced with the standard raycasting stepping algorithm:
    ```c
    step = 1.0 * tex->h / wall->line_height;
    tex_pos = (wall->draw_start - game->screen.h / 2.0 + wall->line_height / 2.0) * step;
    ```
  * In the render loop, `tex_y = (int)tex_pos` is clamped to `[0, tex->h - 1]`, and `tex_pos += step` advances per pixel column.

---

## 4. Complete Memory Leak Elimination & Zero "Still Reachable" Leaks (Valgrind-Verified)
* **Files:** [`src/utils/cleanup.c`](file:///home/mwei/42Projects/Cub3D/src/utils/cleanup.c), [`src/render/init_mlx.c`](file:///home/mwei/42Projects/Cub3D/src/render/init_mlx.c), [`src/render/load_textures.c`](file:///home/mwei/42Projects/Cub3D/src/render/load_textures.c)
* **Problem:**
  * None of the 4 loaded XPM textures (`no_img`, `so_img`, `we_img`, `ea_img`) were freed with `mlx_destroy_image`.
  * MiniLibX display connections were never freed with `mlx_destroy_display(game->mlx)` and `free(game->mlx)` on Linux, leaving massive blocks marked as "still reachable" by Valgrind.
  * If an error occurred after window initialization, MLX resources were leaked.
  * When the parser failed mid-file, the static buffer inside `get_next_line` was left un-freed.
* **Solution:**
  * In `cleanup_game`, added complete deallocation: destroying all 4 texture images, destroying the screen image, destroying the window, calling `mlx_destroy_display(game->mlx)`, and freeing `game->mlx`.
  * Routed `close_game` and `error_exit_game` through `cleanup_game`.
  * Drained `get_next_line` using `consume_file(fd)` on parser error exits to free GNL's internal buffer.
* **Valgrind Output Verification:**
  ```text
  == HEAP SUMMARY ==
      in use at exit: 0 bytes in 0 blocks
    total heap usage: 644 allocs, 644 frees, 106,651 bytes allocated

  All heap blocks were freed -- no leaks are possible
  ERROR SUMMARY: 0 errors from 0 contexts
  ```
  * **Definitely lost:** 0 bytes in 0 blocks
  * **Indirectly lost:** 0 bytes in 0 blocks
  * **Possibly lost:** 0 bytes in 0 blocks
  * **Still reachable:** **0 bytes in 0 blocks**

---

## 5. Player Wall Clipping & Void Walk Prevention
* **File:** [`src/movement/move.c`](file:///home/mwei/42Projects/Cub3D/src/movement/move.c) (`can_move_to`, `is_walkable`)
* **Problem:**
  * Movement only checked `grid[(int)y][(int)x] != '1'`. Without a collision radius, players could stand at `x = 1.000000000001` directly on the wall boundary (`perp_wall_dist` $\approx 0$), causing 32-bit integer overflow in `screen_height / perp_wall_dist`.
  * It did not check for spaces (`' '`) or map boundaries, allowing players to walk outside into empty space.
* **Solution:**
  * Added a player bounding box margin (`margin = 0.2`) checking all 4 corners:
    ```c
    static int can_move_to(t_game *game, double new_x, double new_y)
    ```
  * Added `is_walkable` to check bounds, non-walls (`!= '1'`), and non-spaces (`!= ' '`).

---

## 6. Uneven Strafing Speed
* **File:** [`src/movement/move.c`](file:///home/mwei/42Projects/Cub3D/src/movement/move.c) (`strafe_right`), [`src/movement/keypress_move.c`](file:///home/mwei/42Projects/Cub3D/src/movement/keypress_move.c) (`strafe_left`)
* **Problem:**
  * Forward/backward movement used `dir * speed` (vector length `1.0`).
  * Strafing used `plane * speed` (vector length `0.66`).
  * Strafing was 34% slower than forward/backward walking.
* **Solution:**
  * Computed strafe vectors using the normalized perpendicular vector `(-dir_y, dir_x)` (magnitude `1.0`), making strafe speed identical to walking speed.
  * Implemented `strafe_left(game, speed)` as `strafe_right(game, -speed)` for code reuse.

---

## 7. Division by Zero / NaN in Raycasting Vectors
* **File:** [`src/raycast/vectors.c`](file:///home/mwei/42Projects/Cub3D/src/raycast/vectors.c) (`init_dda`, `calc_wall_height`, `dda_loop`)
* **Problem:**
  * `ray->delta_dist_x = fabs(1 / ray->ray_dir_x)` produced `inf` when `ray_dir == 0.0`. When multiplied by `0.0` in `side_dist`, it produced `NaN`, corrupting comparisons and stalling the raycaster.
  * In `calc_wall_height`, very small or zero `perp_wall_dist` caused division overflow.
  * In `dda_loop`, the loop lacked bounds checking against map dimensions.
* **Solution:**
  * Assigned `1e30` to `delta_dist` when `ray_dir == 0.0` (standard Lodev raycaster safeguard).
  * Guarded `perp_wall_dist < 1e-6` with a floor of `1e-6`.
  * Added grid boundary checks inside `dda_loop` before accessing `game->map.grid[ray->map_y][ray->map_x]`.

---

## 8. Map Parser Whitespace & Enclosure Checks
* **Files:** [`src/parser/parse_map.c`](file:///home/mwei/42Projects/Cub3D/src/parser/parse_map.c), [`src/parser/validate_map.c`](file:///home/mwei/42Projects/Cub3D/src/parser/validate_map.c), [`src/parser/flood_fill.c`](file:///home/mwei/42Projects/Cub3D/src/parser/flood_fill.c)
* **Problem:**
  * `is_line_blank` only checked for `\n` and `\r`. Lines with spaces or tabs (e.g. `"   \n"`) were treated as map lines, causing false enclosure errors.
  * Player spawn cell (`N`, `S`, `E`, `W`) remained on `game->map.grid`, leaving a non-standard character in the grid array.
  * In `flood_fill.c`, when a leak was found, recursion continued needlessly through remaining directions.
* **Solution:**
  * Used `is_empty_line` from `parser_utils.c` to properly detect whitespace-only lines.
  * Reset `game->map.grid[y][x] = '0'` after capturing player spawn coordinates and orientation.
  * Added early returns in `flood_check` as soon as a leak is detected.
  * Used `ft_calloc` in `duplicate_map` to ensure memory is zeroed for safe deallocation on failure.

---

## 9. Keycode Handling (macOS & Linux)
* **File:** [`src/movement/keypress_move.c`](file:///home/mwei/42Projects/Cub3D/src/movement/keypress_move.c)
* **Problem:** Keypress handler only accepted lowercase ASCII codes (e.g. `119` for 'w'). If CapsLock was on or Shift was held, controls stopped responding.
* **Solution:** Supported both lowercase and uppercase keys (`w`/`W`, `s`/`S`, `a`/`A`, `d`/`D`) as well as macOS keycodes (`53` for ESC, `123`/`124` for arrows).

---

## 10. Makefile Header Dependency
* **File:** [`Makefile`](file:///home/mwei/42Projects/Cub3D/Makefile)
* **Problem:** Object files only depended on `.c` files, so modifying `include/cub3d.h` did not cause `make` to recompile outdated objects.
* **Solution:** Added `include/cub3d.h` as a dependency to the `$(OBJ_DIR)/%.o` target.

---

## 11. Norminette Compliance & Architecture Refactoring
* **Scope:** All files in `src/` and `include/` (excluding 42 file headers per instructions).
* **Fixes Implemented:**
  * **Line lengths:** Wrapped all conditionals and calculations across multiple lines to guarantee $\le 80$ columns.
  * **Function lines:** Split functions exceeding 25 lines (e.g., `init_dda` split into `init_step_x` and `init_step_y`, `paint_wall_column` into `draw_slice`).
  * **Variable count:** Limited functions to at most 5 variable declarations.
  * **Function counts per file:** Redistributed functions (`render_walls` into `load_textures.c`, `free_map` into `parser_utils.c`, `rotate_player` and `strafe_left` into `keypress_move.c`) so every single file has $\le 5$ functions.
  * **Formatting:** Converted spaces to tabs in function declarations and parameter alignments in `cub3d.h`.

---

## 12. Edge Case Testing Matrix
The following edge cases were tested and verified leak-free:

| Test Case | Scenario | Expected Result | Valgrind Status |
| --- | --- | --- | --- |
| Normal run | `./cub3D maps/valid.cub` | Renders properly, exits via ESC | 0 leaks, 0 still reachable |
| Showcase map | `./cub3D maps/textures_showcase.cub` | Renders all 4 textures with correct orientation | 0 leaks, 0 still reachable |
| Missing texture | Texture path points to non-existent file | Clean error message and full deallocation | 0 leaks, 0 still reachable |
| Unclosed map | Map has a hole to space/edge | `Error: Map is not enclosed by walls.` | 0 leaks, 0 still reachable |
| Duplicate config | File contains two `NO` or `F` lines | `Error: Invalid configuration element in file.` | 0 leaks, 0 still reachable |
| Missing player | Map has no spawn character | `Error: Map must have one player start position.` | 0 leaks, 0 still reachable |
| Multiple players | Map has more than one spawn character | `Error: Map must have one player start position.` | 0 leaks, 0 still reachable |
| Invalid character | Map contains unknown characters (e.g. `'X'`) | `Error: Map contains invalid characters.` | 0 leaks, 0 still reachable |
| Empty line in map | Grid is separated by an empty line | `Error: Map contains empty line.` | 0 leaks, 0 still reachable |
| Empty line with spaces | Blank line containing spaces inside grid | `Error: Map contains empty line.` | 0 leaks, 0 still reachable |
| Empty file | File has 0 bytes | `Error: Incomplete configuration in file.` | 0 leaks, 0 still reachable |
| Directory as argument | Passing directory `./cub3D dir.cub` | Clean error without crash | 0 leaks, 0 still reachable |
