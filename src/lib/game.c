#include <stdio.h>
#include <stdlib.h>
#include "../include/game.h"

void go_up (Tile **map, int size_map,int array[4][2], int bot){/* Moves a given robot up*/
    // If there are no walls straight ahead or a robot in the next tile, move up one tile
    if (!(is_wall(map, size_map, array[bot][0], array[bot][1], 1)) && map[array[bot][0]-1][array[bot][1]].robot == 64){ // if no wall and no bot
        map[array[bot][0]-1][array[bot][1]].robot = map[array[bot][0]][array[bot][1]].robot; // we copy the robot in the upper tile
        map[array[bot][0]][array[bot][1]].robot = 64; // reset the tile left behind
        array[bot][0] -= 1; // we update the robot coordinate array

        return go_up(map, size_map, array, bot); // recursive
    }
}

void go_right (Tile **map, int size_map, int array[4][2], int bot){ /* Moves a given robot to the right*/
    // If there are no walls straight ahead or a robot in the next tile, move right one tile
    if (!(is_wall(map, size_map, array[bot][0], array[bot][1], 2)) && map[array[bot][0]][array[bot][1]+1].robot == 64){ // if no wall and no bot
        map[array[bot][0]][array[bot][1]+1].robot = map[array[bot][0]][array[bot][1]].robot; // we copy the robot in the right tile
        map[array[bot][0]][array[bot][1]].robot = 64; // reset the tile left behind
        array[bot][1] += 1; // we update the robot coordinate array

        return go_right(map, size_map, array, bot); // recursive
    }
}

void go_down (Tile **map, int size_map, int array[4][2], int bot){ /* Moves a given robot down*/
    // If there are no walls straight ahead or a robot in the next tile, move down one tile
    if (!is_wall(map, size_map, array[bot][0], array[bot][1], 3) && map[array[bot][0]+1][array[bot][1]].robot == 64){ // if no wall and no bot
        map[array[bot][0]+1][array[bot][1]].robot = map[array[bot][0]][array[bot][1]].robot;
        map[array[bot][0]][array[bot][1]].robot = 64; // reset the tile left behind
        array[bot][0] += 1; // we update the robot coordinate array

        return go_down(map, size_map, array, bot); // recursive
    }
}

void go_left (Tile **map, int size_map, int array[4][2], int bot){ /* Moves a given robot to the left*/
    // If there are no walls straight ahead or a robot in the next tile, move left one tile
    if (!is_wall(map, size_map, array[bot][0], array[bot][1], 4) && map[array[bot][0]][array[bot][1]-1].robot == 64){ // if no wall and no bot
        map[array[bot][0]][array[bot][1]-1].robot = map[array[bot][0]][array[bot][1]].robot;
        map[array[bot][0]][array[bot][1]].robot = 64; // reset the tile left behind
        array[bot][1] -= 1; // we update the robot coordinate array

        return go_left(map, size_map, array, bot); // recursive
    }
}

void print_scores(int *array, int size_array){ // Displays the score array
    for (int i = 0; i < size_array; i++){
        printf("Player %d: %d point", i + 1, array[i]);
        if (array[i] > 1)
            printf("s");
        printf("\n");
    }
}

void print_winner(int *array_score, int size_array){
    int max = array_score[0];
    for (int i = 1; i < size_array; i++){// First, find the maximum score
        if (array_score[i] > max)
            max = array_score[i];
    }

    int *array_winner_index = malloc(size_array * sizeof(int)); // Array that will contain the indexes for which array[i] = maximum
    if (array_winner_index == NULL){
        perror("Error ");
        exit(EXIT_FAILURE);
    }

    int count = 0; // Number of ties and actual size of array_winner_index
    for (int i = 0; i < size_array; i++){// A second time to fill the array with the indexes of the winners
        if (array_score[i] == max){
            array_winner_index[count] = i;
            count++;
        }
    }

    if (count == 1)
        printf("The winner is Player %d!\n\n", array_winner_index[0] + 1);
    else{
        printf("The winners are Players ");
        int i;
        for (i = 0; i < count; i++){
            printf("%d", array_winner_index[i] + 1); // print the number of each player in the winner array
            if (i == count - 2){
                printf(" and ");
            }
            else if (i < count - 2)
                printf(", ");
        }
        printf("!\n\n");
    }
    
    free(array_winner_index);
}
