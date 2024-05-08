#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


void print_winner(int *array_score, int size_array){
    int max = array_score[0];
    for (int i=1; i<size_array; i++){// on parcourt une première fois le tableau pour trouver le minimum
        if (array_score[i]>max)
            max = array_score[i];
    }

    int *array_winner_index = malloc(size_array*4); // tableau qui contiendra les index pour lesquels array[i] = mini
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
        printf("The winner is Player %d !\n",array_winner_index[0]+1);
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
    }
    printf(" !\n");

    free(array_winner_index);
}

void main(){
    srand(time(NULL));
    int array[4] = {3,3,3,1};
    print_winner(array,4);
}