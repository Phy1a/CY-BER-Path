#ifndef MAP_H
#define MAP_H

#include "tile.h"
#include "utils.h"

void create_map(Tile **map, int size_map);
void print_row(Tile **map, int row, int size_map);
void print_map(Tile **map, int size_map);
int is_wall(Tile **map, int size_map, int row, int column, int position);
void add_wall(Tile **map, int size_map, int row, int column, int position);
int is_target_valid(Tile **map, int size_map, int row, int column);
void  random_wall(int *array,int size_map);
void add_map_side_walls(Tile **map, int size_map);
void add_map_target(Tile **map, int size_map);
void add_map_robot(Tile **map, int size_map, int array[4][2]);

#endif // MAP_H