#ifndef GAME_H
#define GAME_H

#include "tile.h"
#include "map.h"
#include "utils.h"

void go_up (Tile **map, int size_map,int array[4][2], int bot);
void go_right (Tile **map, int size_map,int array[4][2], int bot);
void go_down (Tile **map, int size_map,int array[4][2], int bot);
void go_left (Tile **map, int size_map,int array[4][2], int bot);
void print_scores(int *array, int size_array);
void print_winner(int *array_score, int size_array);

#endif // GAME_H