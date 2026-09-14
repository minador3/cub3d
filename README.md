*This project has been created as part of the 42 curriculum by mwei.*

# cub3D

A 3D raycasting maze exploration engine inspired by the legendary Wolfenstein 3D (1992), built from scratch in C using **MiniLibX**.

---

## 📖 Description

**cub3D** is a graphics project from the 42 school curriculum designed to explore mathematical principles of raycasting and graphic rendering using a minimal graphical library (MiniLibX). The engine parses a `.cub` scene description file, validates the map environment and wall textures, and renders a pseudo-3D first-person perspective in real time using the **Digital Differential Analysis (DDA)** algorithm.

---

## 🕹️ Controls

| Key | Action |
| --- | --- |
| `W` | Move Forward |
| `S` | Move Backward |
| `A` | Strafe Left |
| `D` | Strafe Right |
| `Left Arrow` | Rotate View Left |
| `Right Arrow` | Rotate View Right |
| `ESC` / `Window Cross` | Clean Exit |

---

## 💻 Instructions

### Compilation
Compile the project with the provided Makefile:
```bash
make
```

### Execution
Run the executable with a valid `.cub` configuration file:
```bash
./cub3D maps/valid.cub
```

Or test with the texture showcase map:
```bash
./cub3D maps/textures_showcase.cub
```

### Clean up
Remove object files:
```bash
make clean
```
Remove object files and executable:
```bash
make fclean
```
Recompile everything:
```bash
make re
```

---

## 📚 Resources

* [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) — Reference for 2D DDA raycasting, camera plane mathematics, and wall texture projection.
* [MiniLibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx) — API guide for window creation, pixel rendering buffers, and X11 hooks.