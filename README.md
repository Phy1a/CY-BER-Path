This code is basically a strategic game, I'll let u discover it if you wish.

Make sure to compile everything like this :

For windows : 

    gcc main.c lib/utils.c lib/map.c lib/tile.c lib/game.c -o my_program

Check file paths if you encounter any error.

For linux :

    gcc -o my_programm main.c lib/utils.c lib/map.c lib/tile.c lib/game.c


It doesn't need any special extention except for a C code runner and all the files contained in lib's folder, include's folder and main.c.

This program generates a game in which several players play, the aim being to reach a randomly designated target with the minimum number of moves. The player with the minimum number of moves goes first, if he reaches for his target with the right number he gets: otherwise, if he reaches but with a different number he only gets one point . And if he doesn't reach his target, everyone takes a point exept him. The difficulty of the game is that the robot moves in a straight line and stops only when it encounters a wall, another robot or another target.

The right reading order to read and understand the program : 
- tile.c
- map.c
- game.c
- utile.c
- main.c

FONCTION USE IN THESE FILES :
  FOR TILE.C:
    This file implements the functions declared in the `tile. h' header file. Here is an explanation 
    detailed for each function: 
    1. **'create_tile' function:** - This function creates a tile with default walls not present (`0'), no robot (`64'), and 
    no target (`0'). 
    2. **Better_tile function:** - This function adjusts the walls of a tile for a cleaner display. - assigns a value of -1 to walls that are already represented     by the adjacent tile wall. 
    3. **`print_colored' function:** - This function displays a color string. - Uses ANSI color codes on Linux systems and library functions 
    Windows (GetStdHandle and SetConsoleTextAttribute) on Windows. 
    4. **Functions `print_tile1', `print_tile2', `print_tile3`:** - These functions display the first, second and third row of a tile, respectively. 
    - They take into account the walls and special elements (robot, target) present on the tile. - They use print_colored to display text in colour according to         the platform. 
    These functions are crucial for the visual representation of tiles in the game. They allow 
    correctly initialize a tile, adjust it for optimal display and display each part 
    tile with appropriate elements, such as walls, robots and targets. 

