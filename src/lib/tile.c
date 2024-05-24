#include <stdio.h>
#include "../include/tile.h"

Tile create_tile(){
    Tile a;

    // 0 = pas de mur, 1 = mur et -1 = même mur que la case adjacente
    // à l'initilisation d'une case, celle-ci n'a as de mur
    a.upper_wall = 0;  
    a.lower_wall = 0; 
    a.right_wall = 0;
    a.left_wall = 0; 

    a.robot = 64; // = pas de robot

    a.target = 0; // = pas de cible

    return a;
}

void better_tile(Tile *a, int row, int column){/* This function formats the tiles for a cleaner display, it assigns values of -1 when the wall 
is already represented by the wall of the adjacent tile, without this function, we would have displays like this: Tile A || Tile B 
with 2 walls side by side*/
    if (row){ // if the row is not 0
        a->upper_wall = -1;// the value -1 tells the display to show nothing
    }
    if(column){ // if the column is not 0
        a->left_wall = -1; 
    }
}

#ifdef _WIN32
void print_colored(char *text, int color) { // allows displaying a string in color
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    printf("%s", text);
    SetConsoleTextAttribute(hConsole, 7);
}
#elif defined(__linux__)
void print_colored(const char *text, const char *color_code) {
    printf("%s%s\033[0m", color_code, text);
}
#endif


void print_tile1(Tile a){// displays the first line of tile a
    if (a.upper_wall==1)
        print_colored("-----", BLUE);
    if (a.upper_wall==0)
        printf("     ");
}

void print_tile2(Tile a){ // displays the second line of tile a

    if (a.left_wall==1)
        print_colored("| ",BLUE);
    else if(a.left_wall==0)
        printf("  ");
    else if (a.left_wall==-1)
        printf(" ");
    // if a.left_wall==-1, show nothing
        
    
    if(a.robot != 64){ // cannot use print_colored here because %c
        #ifdef _WIN32 // for Windows
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, RED);
        printf(" %c", a.robot);
        SetConsoleTextAttribute(hConsole, 7);
        #elif defined(__linux__)// for linux
        printf("%s %c\033[0m", RED ,a.robot);
        #endif
    }
    else if (a.target){
        if (a.target<10)
            printf(" ");
        
        // cannot use print_colored here because %d
        #ifdef _WIN32 // for Windows
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, GREEN);
        printf("%d", a.target);
        SetConsoleTextAttribute(hConsole, 7);
        #elif defined(__linux__)// for linux
        printf("%s%d\033[0m", GREEN ,a.target);
        #endif
    }
    else{
        printf("  ");
    }

    if (a.right_wall==1)
        print_colored(" |",BLUE);
    else if(a.right_wall==-1)
        printf(" ");
    else if(a.right_wall==0)
        printf("  ");
}

void print_tile3(Tile a){ // displays the third line of tile a
    if (a.lower_wall==1)
        print_colored("-----", BLUE);
    else if (a.lower_wall==0)
        printf("     ");
    // if a.lower_wall==-1, show nothing 
}
