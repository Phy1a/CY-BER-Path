#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/utils.h"

int rdm(int mini, int maxi){ // generates a random integer between min and max inclusive
    return rand() % (maxi - mini + 1) + mini;
}

void getint(int *result){ /* function that asks the user for an integer and ensures the validity of the response */
    int error_detector = scanf("%d", result);
    while (!error_detector){
        printf("Please enter a number: ");
        while(getchar() !='\n');  // removes all characters stuck in the buffer as unused by scanf
        // so that we can re-enter a new value in number_player, without this the program loops
        error_detector = scanf("%d", result);
    }
    while(getchar() !='\n'); // clears the buffer in case the user enters something like "2sjhdfksdhf", the 2 is taken but not the rest
}

void get_positive_int(int *result){ /* function that asks the user for a strictly positive number 
and ensures the validity of the response */
    getint(result);
    while (*result <= 0){
        printf("Please enter a positive number: ");
        getint(result);
    }
}

int pick_min_array(int *array, int size_array, int *player_index){// Randomly selects the player who will play among those tied

    int mini = array[0];
    for (int i = 1; i < size_array; i++){// first loop through the array to find the minimum

        if (mini == -1 && array[i] > mini) // in case array[0] = -1;
             mini = array[i];

        if (array[i] < mini && array[i] != -1) // -1 means the player has not found solutions
            mini = array[i];
    }
    
    int *array_mini_index = malloc(size_array * sizeof(int)); // array that will contain the indices for which array[i] = mini
    if (array_mini_index == NULL){
        perror("Error ");
        exit(EXIT_FAILURE);
    }

    if (mini == -1){
        *player_index = 0;
        free(array_mini_index);
        return mini;
    }

    int count = 0; // Number of ties and actual size of array_mini_index
    for (int i = 0; i < size_array; i++){// second loop to fill the array with the indices of ties
        if (array[i] == mini){
            array_mini_index[count] = i;
            count++;
        }
    }

    int roll_dices = rdm(0, count - 1); // random choice of the selected player
    *player_index = array_mini_index[roll_dices];

    free(array_mini_index);

    return mini;
}
