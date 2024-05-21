#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/utils.h"

int rdm(int mini, int maxi){ // génère un entier aléatoire entre min et max inclus
    return rand() % (maxi - mini + 1) + mini;
}

void getint(int *result){ /* fonction qui demande un entier à l'utilisateur et s'assure de la validité de la réponse*/
    int error_detector = scanf("%d",result);
    while (!error_detector){
        printf("Please enter a fitting number : ");
        while(getchar() !='\n');  // enlève tous les caractères bloqués dans le tamponn car inutilisé par scanf
        //pour qu'on puisse reentrer une nouvelle valeur dans number_player, sans cela le programme boucle
        error_detector = scanf("%d",result);
    }
}

void get_positive_int(int *result){ /* fonction qui demande un nombre strictement positif 
à l'utilisateur et s'assure de la validité de la réponse*/
    getint(result);
    while (*result<=0){
        printf("Please enter a fitting number : ");
        getint(result);
    }
}

int pick_min_array(int *array,int size_array,int *player_index){// Selection au hasard le joueur qui jouera parmi ceux à égalité

    int mini = array[0];
    for (int i=1; i<size_array; i++){// on parcourt une première fois le tableau pour trouver le minimum

        if (mini==-1 && array[i]>mini) // dans le cas où array[0] = -1;
             mini = array[i];

        if (array[i]<mini && array[i] != -1) // -1 veut dire que le joueur n'a pas trouvé de solutions
            mini = array[i];
    }
    
    int *array_mini_index = malloc(size_array*4); // tableau qui contiendra les index pour lesquels array[i] = mini
    if (array_mini_index==NULL){
        perror("Error ");
        exit(EXIT_FAILURE);
    }

    if (mini == -1){
        *player_index = 0;
        return mini;
    }

    int count = 0; // Nombre d'égalité et tailles réelle du tableau array_mini_index
    for (int i=0; i<size_array; i++){// une deuxième fois pour remplir le tableau avec les index des égalités
        if (array[i]==mini){
            array_mini_index[count]=i;
            count ++;
        }
    }

    int roll_dices = rdm(0,count-1); // choix aléatoire du joueurs choisi
    *player_index = array_mini_index[roll_dices];

    free(array_mini_index);

    return mini;
}
