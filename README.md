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
- utils.c
- main.c

## Fonctions used in these files :


### tile.c :

This file implements the functions declared in the 'tile.h' header file. Here is an explanation 
detailed for each function: 

1. 'create_tile' : This function creates a tile with default walls not present ('0'), no robot ('64'), and no target ('0'). 

2. 'better_tile' : This function adjusts the walls of a tile for a cleaner display. Assigns a value of 1 to walls that are already represented by the adjacent tile wall. 

3. 'print_colored' : This function displays a color string. Uses ANSI color codes on Linux systems and library functions 
Windows (GetStdHandle and SetConsoleTextAttribute) on Windows. 

4. Functions 'print_tile1', 'print_tile2', 'print_tile3': These functions display the first, second and third row of a tile, respectively.
They take into account the walls and special elements (robot, target) present on the tile.They use print_colored to display text in colour according to the platform.
These functions are crucial for the visual representation of tiles in the game. They allow 
correctly initialize a tile, adjust it for optimal display and display each part 
tile with appropriate elements, such as walls, robots and targets. 


### map.c :

implementation of functions declared in the 'map. h' header file. 

1. 'create_map' : This function creates a game map by filling each tile with default values at 
using create_tile then adjusting the walls around each tile with better_tile. It also adds walls to the edges of the card. 

2. 'print_row' : This function displays a line from the map on the screen. It scans each column in the row and displays each tile using the 
functions 
print_tile1, print_tile2 and print_tile3 for each part of the tile. It also manages the display of walls between lines.

3. 'print_map': This function displays the complete map by calling 'print_row' for each line of the map. 

4. 'is_wall': This function checks if a tile has a wall in a specific direction. Returns 1 if a wall is present and 0 if not. 

5. 'add_wall' : This function adds a wall to a tile in a specific direction. It also updates the walls of the adjacent tile based on the direction of the added wall. 

6. 'is_target_valid' : This function checks if a given position is valid to place a target. Checks that the target is not in contact with other targets or random walls. 

7. 'random_wall' : This function fills an array with random coordinates for walls. 

8. 'add_map_side_walls' : This function adds walls to the sides of the board. 

9. 'add_map_target' : This function adds targets to the map by choosing valid positions. 

10. 'add_map_robot' : This function adds robots to the map by choosing valid positions. 

These functions are essential for creating, displaying and manipulating the card in the game. 
They are the basis of the functioning of the the interaction of players with the gaming environment.


### game.c : 

a C implementation of the functions declared in the 'game.h' header file. 

1. Motion ('go_up', 'go_right', 'go_down', 'go_left'): These functions move a robot on the map in different directions: upwards, to the right, to the bottom and left. They first check if there is no wall or other robot in the next box, then 
move the robot accordingly. Each function uses a recursive approach to allow the robot to move as long as it 
is no obstacle. 

2. 'print_scores' : Displays player scores on screen. Scans the scoreboard and displays each score with the corresponding player ID. 

3. Function 'print_winner: Determines the winner or winners based on scores. Finds the maximum score among all players. Identifies players with this maximum score and displays them as winners. In the event of a tie, all players with the maximum score are designated as winners. 

4. Using dynamic memory: The 'print_winner' function uses dynamic memory allocation to store indexes of 
players with the maximum score. This memory is released at the end of the function to avoid memory leaks. 

These functions are an essential logical part of movement management and 
determining scores and the winner in the game.

### utils.c : 

This file contains the implementations of the functions declared in the 'utils. h' header file. 

Here is a detailed explanation of each function: 

1. rdm This function generates a random integer between a minimum value and a value 
maximum included. 

2. 'getint: This function asks the user to enter an integer and ensures that the answer is valid. If the user between anything other than an integer, they are prompted to try again until an entry valid be provided. 

3. "get_positive_int" : This function asks the user to enter a strictly positive integer and ensures the validity of the response. It uses the 'getint' function to obtain the user’s input, then checks if it is strictly positive.

4. 'pick_min_array' : This function randomly selects the player who will play among those who are tied. It takes as input an array of integers and its size, then finds the minimum value in the table. If the minimum value is '1', it means that no player has found solutions, and the function returns '-1'. If several players have the same minimum value, the function randomly selects one of them. 
 Returns the minimum value found.

These functions are useful for generating random numbers, getting user inputs and 
randomly select items from a table, and they contribute to the right 
game operation by allowing interactions with the user and providing 
equalities resolution mechanisms.


### main.c :

The code provided is a C program that implements a board game where players 
control robots to hit targets on a map 

1. Initialization and Configuration: The main() function starts by initializing the random number generator in 
using the system clock ('srand(time(NULL))'), which guarantees sequences of numbers 

different random at each run. Users are asked to enter the number of players, the number of rounds and the 
level of difficulty. Checks are made to ensure that the values entered 
are valid and within the specified ranges. 

2. Creating the Game Card: The size of the card is determined randomly in a predefined range. A two - dimensional array of tiles is allocated in memory to represent the map. - Walls, targets and robots are randomly placed on the map using 
functions provided in the corresponding header files. The map is displayed to allow players to view the game environment. 

3. Game Rounds Progress: For each game round, a robot is randomly selected and a target is 
designated for this round. Players have limited time to observe the current configuration before taking 
their movement decisions. The movements proposed by the players are recorded and evaluated to determine the 
minimum number of moves required to reach target. Each player moves in order, following the rules of the game. Player scores are updated based on the outcome of each round. 

4. Winner Determination: At the end of all rounds, the player with the highest score is declared the winner. The winner is displayed on the screen to inform players of the final outcome of the game. 

5. Free Allocated Memory: Once the game is over, the memory allocated for st
