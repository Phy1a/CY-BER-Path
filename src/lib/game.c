#include <stdio.h>
#include <stdlib.h>
#include "../include/game.h"


void go_up (Tile **map, int size_map,int array[4][2], int bot){/* Déplace un robot donné vers le haut*/
    // Si il n'y a pas de murs droit devant ou de robot dans la case suivante, on se déplace d'une case
    if (!(is_wall(map,size_map,array[bot][0],array[bot][1],1)) && map[array[bot][0]-1][array[bot][1]].robot == 64){
        map[array[bot][0]-1][array[bot][1]].robot = map[array[bot][0]][array[bot][1]].robot;
        map[array[bot][0]][array[bot][1]].robot = 64;
        array[bot][0] -= 1;

        return go_up(map,size_map,array,bot); // récursif
    }
}

void go_right (Tile **map, int size_map,int array[4][2], int bot){ /* Déplace un robot donné vers la droite*/
    // Si il n'y a pas de murs droit devant ou de robot dans la case suivante, on se déplace d'une case
    if (!(is_wall(map,size_map,array[bot][0],array[bot][1],2))&& map[array[bot][0]][array[bot][1]+1].robot == 64){
        map[array[bot][0]][array[bot][1]+1].robot = map[array[bot][0]][array[bot][1]].robot;
        map[array[bot][0]][array[bot][1]].robot = 64;
        array[bot][1] += 1;

        return go_right(map,size_map,array,bot); // récursif
    }
}

void go_down (Tile **map, int size_map,int array[4][2], int bot){ /* Déplace un robot donné vers le bas*/
    // Si il n'y a pas de murs droit devant ou de robot dans la case suivante, on se déplace d'une case
    if (!is_wall(map,size_map,array[bot][0],array[bot][1],3) && map[array[bot][0]+1][array[bot][1]].robot == 64){
        map[array[bot][0]+1][array[bot][1]].robot = map[array[bot][0]][array[bot][1]].robot;
        map[array[bot][0]][array[bot][1]].robot = 64;
        array[bot][0] += 1;

        return go_down(map,size_map,array,bot); // récursif
    }
}

void go_left (Tile **map, int size_map,int array[4][2], int bot){ /* Déplace un robot donné vers la gauche*/
    // Si il n'y a pas de murs droit devant ou de robot dans la case suivante, on se déplace d'une case
    if (!is_wall(map,size_map,array[bot][0],array[bot][1],4) && map[array[bot][0]][array[bot][1]-1].robot == 64){
        map[array[bot][0]][array[bot][1]-1].robot = map[array[bot][0]][array[bot][1]].robot;
        map[array[bot][0]][array[bot][1]].robot = 64;
        array[bot][1] -= 1;

        return go_left(map,size_map,array,bot); // récursif
    }
}

void print_scores(int *array, int size_array){ // Affiche le tableau des scores
    for (int i=0; i<size_array;i++){
        printf("Player %d : %d point",i+1,array[i]);
        if (array[i]>1)
            printf("s");
        printf("\n");
    }
}

void print_winner(int *array_score, int size_array){
    int max = array_score[0];
    for (int i=1; i<size_array; i++){// on parcourt une première fois le tableau pour trouver le maximum
        if (array_score[i]>max)
            max = array_score[i];
    }

    int *array_winner_index = malloc(size_array*4); // tableau qui contiendra les index pour lesquels array[i] = maximum
    if (array_winner_index==NULL){
        perror("Error ");
        exit(EXIT_FAILURE);
    }

    int count = 0; // Nombre d'égalité et tailles réelle du tableau array_winner_index
    for (int i=0; i<size_array; i++){// une deuxième fois pour remplir le tableau avec les index des gagnants
        if (array_score[i]==max){
            array_winner_index[count]=i;
            count ++;
        }
    }

    if (count==1)
        printf("The winner is Player %d !\n\n",array_winner_index[0]+1);
    else{
        printf("The winners are Players ");
        int i;
        for (i = 0; i<count;i++){
            printf("%d",array_winner_index[i]+1);
            if (i==count-2){
                printf(" and ");
            }
            else if(i<count-2)
                printf(", ");
            }
        printf(" !\n\n");
    }
    
    free(array_winner_index);
}
