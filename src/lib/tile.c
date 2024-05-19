#include <stdio.h>
#include "tile.h"

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

void better_tile(Tile *a, int row, int column){/* Cette fonction formate les cases pour un affichage épurer, elle attribue des valeurs -1 quand le mur 
est déjà matérialiser par le mur de la case d'à côté, sans cette fonction on aurait des affichages comme ceci : Case A || Case B 
avec 2 murs côte à côte*/
    if (row){ // si la ligne ne vaut pas 0
        a->upper_wall = -1;// la valeur -1 dit à l'affichage de ne rien afficher
    }
    if(column){ // si la colonne ne vaut pas 0
        a->left_wall = -1; 
    }
}

#ifdef _WIN32
void print_colored(char *text, int color) { // permet de faire d'afficher une chaîne de caractère en couleur
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


void print_tile1(Tile a){// affiche la première ligne de la case a
    if (a.upper_wall==1)
        print_colored("-----", BLUE);
    if (a.upper_wall==0)
        printf("     ");
}

void print_tile2(Tile a){ // affiche la deuxième ligne de la case a

    if (a.left_wall==1)
        print_colored("| ",BLUE);
    else if(a.left_wall==0)
        printf("  ");
    else if (a.left_wall==-1)
        printf(" ");
    // si a.left_wall==-1 on n'affiche rien
        
    
    if(a.robot != 64){ // on ne peut pas utiliser print_colored ici car %c
        #ifdef _WIN32 // pour Windows
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, RED);
        printf(" %c", a.robot);
        SetConsoleTextAttribute(hConsole, 7);
        #elif defined(__linux__)// pour linux
        printf("%s %c\033[0m", RED ,a.robot);
        #endif
    }
    else if (a.target){
        if (a.target<10)
            printf(" ");
        
        // on ne peut pas utiliser print_colored ici car %d
        #ifdef _WIN32 // pour Windows
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, GREEN);
        printf("%d", a.target);
        SetConsoleTextAttribute(hConsole, 7);
        #elif defined(__linux__)// pour linux
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

void print_tile3(Tile a){ // affiche la troisième ligne de la case a
    if (a.lower_wall==1)
        print_colored("-----", BLUE);
    else if (a.lower_wall==0)
        printf("     ");
    // si a.lower_wall==-1 on n'affiche rien 
}
