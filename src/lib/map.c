#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/map.h"

void create_map(Tile **map, int size_map){
    Tile a;

    for (int row = 0; row < size_map; row++){
        for (int column = 0; column < size_map; column++){
            a = create_tile();
            better_tile(&a, row, column);
            map[row][column] = a;
        }
    }
    
    for (int column = 0; column < size_map; column++) // Adds the walls at the top of the first row
        map[0][column].upper_wall = 1;

    for (int column = 0; column < size_map; column++) // Adds the walls at the bottom of the last row
        map[size_map - 1][column].lower_wall = 1;
    
    for (int row = 0; row < size_map; row++) // Adds the left walls of the first column
        map[row][0].left_wall = 1;

    for (int row = 0; row < size_map; row++) // Adds the right walls of the last column
        map[row][size_map - 1].right_wall = 1;
}


void print_row(Tile **map, int row, int size_map){ // Prints the row `row` of the grid `map` of size `size_map`
    for(int column = 0; column < size_map; column++)
        print_tile1(map[row][column]);
    
    // Visual correction
	if (map[row][0].upper_wall != -1) // If the first wall is -1, then the entire row of walls is -1
		(map[row][size_map - 1].upper_wall) ? print_colored("-\n", BLUE) : printf(" \n");

    for(int column = 0; column < size_map; column++){
        print_tile2(map[row][column]);
    }
    printf("\n");
    
    for(int column = 0; column < size_map; column++){
        print_tile3(map[row][column]);
    }

    // Visual correction
    if (map[row][size_map - 1].lower_wall == 1)
        print_colored("-", BLUE);
    else if (map[row][size_map - 1].lower_wall == 0)
        printf(" ");

    printf("\n");
}


void print_map(Tile **map, int size_map){
    for (int row = 0; row < size_map; row++){ // Prints all rows
        print_row(map, row, size_map);
    }
    printf("\n");
}


int is_wall(Tile **map, int size_map, int row, int column, int position){/* Returns 1 if there is already a wall, 0 otherwise */
	switch (position){
		case 1: // Upper wall
			if(map[row][column].upper_wall == -1) // Check if the tile above has a lower wall
				return (map[row - 1][column].lower_wall == 1) ? 1 : 0;
			else
				return (map[row][column].upper_wall == 1) ? 1 : 0;
			break;
			
		case 2: // Right wall
			return (map[row][column].right_wall == 1) ? 1 : 0;
			break;

		case 3: // Lower wall
			return (map[row][column].lower_wall == 1) ? 1 : 0;
			break;

		case 4: // Left wall
			if(map[row][column].left_wall == -1)
				return (map[row][column - 1].right_wall == 1) ? 1 : 0; // Check if the tile to the left has a right wall
			else
				return (map[row][column].left_wall == 1) ? 1 : 0;
			break;	
	}
}


void add_wall(Tile **map, int size_map, int row, int column, int position){/* Adds the wall to the desired tile 
and in the desired position (upper, right, lower, left) = position (1, 2, 3, 4)
Also adjusts the wall of the neighboring tile */
	if (!is_wall(map, size_map, row, column, position)){ // Check that there is no wall already
		switch (position){
			case 1: // Add upper wall
				(map[row][column].upper_wall == -1) ? (map[row - 1][column].lower_wall = 1) : (map[row][column].upper_wall = 1);
				break;
			case 2: // Add right wall
				(map[row][column]).right_wall = 1;
				break;
			case 3: // Add lower wall
				(map[row][column]).lower_wall = 1;
				break;
			case 4: // Add left wall
				(map[row][column].left_wall == -1) ? (map[row][column - 1].right_wall = 1) : (map[row][column].left_wall = 1);
				break;
		}
	}
}


int is_target_valid(Tile **map, int size_map, int row, int column){/* Checks if the given location is valid or not
Returns 0 (invalid) or 1 (valid) */

    // Check that targets are not in contact with random walls of borders to avoid any issues
    if(row == 1){
        if(is_wall(map, size_map, 0, column, 2) || is_wall(map, size_map, 0, column, 4))
            return 0;
    }
    else if(row == size_map - 2){
        if(is_wall(map, size_map, size_map - 1, column, 2) || is_wall(map, size_map, size_map - 1, column, 4))
            return 0;
    }
    if(column == 1){
        if(is_wall(map, size_map, row, 0, 1) || is_wall(map, size_map, row, 0, 3))
            return 0;
    }
    else if(column == size_map - 2){
        if(is_wall(map, size_map, row, size_map - 1, 1) || is_wall(map, size_map, row, size_map - 1, 3))
            return 0;
    }

    if(map[row][column].target) // Check if the chosen tile does not already contain a target
        return 0;

    // Check if the tiles above do not contain targets
    if(map[row - 1][column - 1].target || map[row - 1][column].target || map[row - 1][column + 1].target)
        return 0;

    // Check if the tiles below do not contain targets
    if(map[row + 1][column - 1].target || map[row + 1][column].target || map[row + 1][column + 1].target)
        return 0;
    
    // Check if the tiles beside do not contain targets
    if(map[row][column - 1].target || map[row][column + 1].target)
        return 0;
    
    return 1; // Otherwise, return 1 for true
}


void random_wall(int *array, int size_map){ // Fills an array with random coordinates for walls
    do{
        array[0] = rdm(2, size_map - 2);
        array[1] = rdm(2, size_map - 2);
    }
    while(abs(array[0] - array[1]) < 2); // Two adjacent walls are unusable for players
}


void add_map_side_walls(Tile **map, int size_map){
    int array[2] = {0}; // Array of coordinates for walls on each side of the board

    random_wall(array, size_map); // Change random walls for each side of the grid, 4 times
    for(int i = 0; i < 2; i++)
        add_wall(map, size_map, 0, array[i], 4); // Add 2 upper walls to the left

    random_wall(array, size_map); 
    for(int i = 0; i < 2; i++)
        add_wall(map, size_map, size_map - 1, array[i], 4); // Add 2 lower walls to the left
    
    random_wall(array, size_map);
    for(int i = 0; i < 2; i++)
        add_wall(map, size_map, array[i], 0, 1); // Add 2 left walls to the top

    random_wall(array, size_map);
    for(int i = 0; i < 2; i++)
        add_wall(map, size_map, array[i], size_map - 1, 1); // Add 2 right walls to the top
}


void add_map_target(Tile **map, int size_map){
    int row, column, a;
    for (int n = 1; n < 17; n++){ // Generates valid target coordinates
        do { 
            row = rdm(1, size_map - 2);
            column = rdm(1, size_map - 2);
        } while (!is_target_valid(map, size_map, row, column)); 

        map[row][column].target = n; // Add them to the board
        
        a = rdm(1, 4);
        add_wall(map, size_map, row, column, a); // 1st wall around the target, random (=a)
        add_wall(map, size_map, row, column, a % 4 + 1); // 2nd wall in the next position
    }
}


void add_map_robot(Tile **map, int size_map, int array[4][2]){
    int row, column;
    for (int i = 0; i < 4; i++){ // Generates valid robot coordinates
        do { 
            row = rdm(0, size_map - 1);
            column = rdm(0, size_map - 1);
        } while (map[row][column].robot != 64); // Check if the tile does not already have a robot
        array[i][0] = row;
        array[i][1] = column;
        map[row][column].robot = i + 65;
    }
}