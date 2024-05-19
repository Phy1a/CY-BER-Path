#ifndef TILE_H
#define TILE_H

#ifdef _WIN32
#include <windows.h>
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define BLUE FOREGROUND_BLUE
void print_colored(char *text, int color);
#elif defined(__linux__)
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
void print_colored(const char *text, const char *color_code);
#endif

typedef struct {
    int upper_wall;
    int lower_wall;
    int right_wall;
    int left_wall;
    int target;
    char robot;
} Tile;

Tile create_tile();
void better_tile(Tile *a, int row, int column);
void print_tile1(Tile a);
void print_tile2(Tile a);
void print_tile3(Tile a);

#endif // TILE_H