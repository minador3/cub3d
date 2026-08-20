# cub3D: Step-by-Step Implementation Plan & TODO List

This document provides a precise, actionable roadmap for developing **cub3D**. It is structured to help you start from scratch, build core modules incrementally, and test components separately before full integration.

---

## 📋 Phase 0: Setup and Shared Foundation

Before writing game logic, establish a solid C project structure and compile a basic window.

- [ ] **0.1. Directory Structure Setup**
  Create the folder layout:
  ```text
  cub3d/
  ├── include/          # Header files (.h)
  ├── src/              # Source files (.c)
  │   ├── parser/       # Parsing & validation
  │   ├── raycast/      # Math & DDA engine
  │   ├── render/       # MLX window & pixel drawing
  │   └── utils/        # Memory cleanup & helpers
  ├── textures/         # .xpm texture assets
  ├── maps/             # Test .cub maps
  ├── libft/            # (Optional) Copy of libft
  └── Makefile          # Compile with -Wall -Wextra -Werror
  ```
- [ ] **0.2. MiniLibX Installation & Setup**
  * Download or clone MiniLibX (depending on whether you are on Linux or macOS).
  * Configure your Makefile to compile and link MiniLibX (`-lmlx -lXext -lX11 -lm` for Linux).
- [ ] **0.3. Design the Shared Data Structure (`cub3d.h`)**
  Create a single header `include/cub3d.h` containing:
  * `t_img`: For MLX images (double buffering).
  * `t_tex`: For loaded wall texture XPM files (NO, SO, WE, EA paths and data).
  * `t_map`: For map configuration (grid 2D array, ceiling/floor colors, width, height).
  * `t_player`: Position (`x`, `y`), direction vector (`dir_x`, `dir_y`), and camera plane (`plane_x`, `plane_y`).
  * `t_game`: Main structure holding pointers to the MLX instance, window instance, structures above, and input state.

---

## 📂 Phase 1: Configuration & Map Parser (Teammate A)

The parser reads the `.cub` scene description and validates it. It must output clean errors (`Error\n...`) and prevent memory leaks.

- [ ] **1.1. File Verification**
  * Check that the file ends with the `.cub` extension.
  * Attempt to open the file; if it fails, display an error message and exit cleanly.
- [ ] **1.2. Parse Identifiers (Textures & Colors)**
  * Parse North (`NO`), South (`SO`), West (`WE`), and East (`EA`) texture paths.
  * Parse Floor (`F`) and Ceiling (`C`) colors as RGB coordinates (validate range `[0, 255]`).
  * Ensure identifiers can be in any order, separated by empty lines or spaces.
  * Ensure all 6 elements are present before reading the map.
- [ ] **1.3. Parse Map Grid**
  * Read the grid lines (always at the end of the file).
  * Convert spaces within the map into a uniform padding format if necessary.
  * Verify map characters: only `0`, `1`, `N`, `S`, `E`, `W`, or spaces.
  * Ensure exactly one starting position (`N`, `S`, `E`, `W`) is present. Save player orientation.
- [ ] **1.4. Map Validation (Flood Fill)**
  * Verify that the map is completely closed/surrounded by walls (`1`).
  * Implement a **flood-fill** validation algorithm checking that empty space `0` (or player start) never leaks to the map edge or spaces.
- [ ] **1.5. Clean Exit & Deallocation**
  * Implement an error handler that prints `Error` followed by a descriptive message.
  * Write a function to free all allocations (2D map array, texture strings/images) on failure or when ESC/close window is triggered.

---

## 📐 Phase 2: Raycasting Engine & Math (Teammate B)

The core math engine calculates the distance from the player to the nearest wall along a grid using Digital Differential Analysis (DDA).

- [ ] **2.1. Basic Vector & Camera Math Setup**
  * Set initial player direction and camera plane based on spawn orientation:
    * **North:** direction $(0, -1)$, plane $(0.66, 0)$
    * **South:** direction $(0, 1)$, plane $(-0.66, 0)$
    * **East:** direction $(1, 0)$, plane $(0, 0.66)$
    * **West:** direction $(-1, 0)$, plane $(0, -0.66)$
- [ ] **2.2. DDA Loop (Digital Differential Analysis)**
  For each vertical column of the screen (from `x = 0` to `width - 1`):
  * Calculate ray position and direction.
  * Find the player's starting map cell box.
  * Compute initial distance to next grid lines (`side_dist_x`, `side_dist_y`).
  * Perform DDA steps to step through grid squares until a wall (`1`) is hit.
- [ ] **2.3. Wall Projection Calculation**
  * Compute the perpendicular ray distance (to avoid fisheye lens effect).
  * Calculate line height to render on screen: $\text{height} = \text{screen\_height} / \text{perp\_wall\_dist}$.
  * Determine the vertical start and end pixels to draw.

---

## 🎨 Phase 3: Rendering & Textures (Teammate B)

Translate raycast math into pixels on the screen using MiniLibX images.

- [ ] **3.1. Double Buffering Setup**
  * Initialize an MLX image buffer using `mlx_new_image`.
  * Write a pixel-pushing helper function: `my_mlx_pixel_put(t_img *img, int x, int y, int color)`.
- [ ] **3.2. Ceiling and Floor Painting**
  * Paint the upper half of each column with the Ceiling color and lower half with the Floor color.
- [ ] **3.3. Wall Texture Mapping**
  * Load texture files (`.xpm`) into MLX images using `mlx_xpm_file_to_image` and extract their pixel data.
  * Identify which wall face was hit (North, South, East, West).
  * Calculate the texture coordinates (`tex_x`, `tex_y`) corresponding to the hit location on the wall.
  * Scale and copy pixels from the source texture to the screen buffer.
- [ ] **3.4. Render Loop Hook**
  * Set up `mlx_loop_hook` to redraw the frame only when state changes or on every frame.

---

## ⌨️ Phase 4: Inputs, Controls & Collisions

Manage player interaction smoothly.

- [ ] **4.1. Key Hooks Registration**
  * Bind keys using `mlx_hook` (KeyPress and KeyRelease) to handle multiple keys pressed simultaneously.
- [ ] **4.2. Translation Controls (W, A, S, D)**
  * Calculate new movement vectors:
    * `W`/`S`: forward/backward along the direction vector.
    * `A`/`D`: strafe left/right along the perpendicular direction vector.
  * Apply simple wall collision check: only move the player's coordinate if the new position is not inside a wall (`1`).
- [ ] **4.3. Rotation Controls (Left/Right Arrows)**
  * Rotate the direction vector and camera plane vector using a 2D rotation matrix:
    $$x' = x \cos(\theta) - y \sin(\theta)$$
    $$y' = x \sin(\theta) + y \cos(\theta)$$
- [ ] **4.4. Clean Window Close**
  * Bind `ESC` (Key Code `53` or `65307`) and the red cross button (Hook `17` - DestroyNotify) to cleanly free memory and exit with `exit(0)`.

---

## 🌟 Phase 5: Testing & Integration

- [ ] **5.1. Leak Checking**
  * Run the game through Valgrind: `valgrind --leak-check=full --show-leak-kinds=all ./cub3D maps/valid.cub`.
  * Ensure no leaks occur when exiting via ESC, the window cross, or on validation errors.
- [ ] **5.2. Edge Cases**
  * Test empty maps, open maps, maps with multiple players, invalid texture paths, invalid colors.
  * Test screen resizing (optional but recommended for stability).

---

## 🎁 Phase 6: Optional Bonuses (Once Mandatory is Perfect)

- [ ] **6.1.** Wall collisions (sliding along walls).
- [ ] **6.2.** Minimap system showing player location/view cone.
- [ ] **6.3.** Doors that open and close.
- [ ] **6.4.** Sprites/animated objects in the scene.
- [ ] **6.5.** Mouse rotation controls.
