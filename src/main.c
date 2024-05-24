#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/tile.h"
#include "include/map.h"
#include "include/game.h"
#include "include/utils.h"


void main(){

	srand(time(NULL));
    
    // Request values from users, robust code ensures consistent values are entered
    int number_player;
    printf("Please enter a number of players: ");
    get_positive_int(&number_player);
    
    int rounds;
    printf("Please enter a number of rounds: ");
    get_positive_int(&rounds);

    int difficulty_level, timer;
    do {
        printf("Please enter a difficulty level (1 to 4): ");
        get_positive_int(&difficulty_level);
    } while(difficulty_level > 4);
    printf("%d Players, %d rounds, difficulty %d\n", number_player, rounds, difficulty_level);
    
    printf("\n");
    if (difficulty_level == 1)
        timer = 40; // in seconds
    else if (difficulty_level == 2)
        timer = 30; // in seconds
    else if (difficulty_level == 3)
        timer = 20; // in seconds
    else
        timer = 10; // in seconds
    

    int size_map = rdm(15,20); // Random choice of board size
    Tile **map = malloc(size_map * sizeof(Tile *)); // 2D array of tiles, allocate space for rows
    // contains pointers to Tiles
    if (map == NULL) {
        perror("Error \n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size_map; i++) {
        map[i] = malloc(size_map * sizeof(Tile)); // for each row, allocate space for columns
        if (map[i] == NULL) {
            perror("Error \n");
            exit(EXIT_FAILURE);
        }
    }

    int array_bot[4][2]; // array containing the coordinates of each robot

    // Creation of the board
    create_map(map, size_map);
    add_map_side_walls(map, size_map);
    add_map_target(map, size_map);
    add_map_robot(map, size_map, array_bot);
    print_map(map, size_map);

    int pick_robot, pick_target, player_index, minimum_moves, player_order, situation;

    int *scores_player_array = malloc(number_player * 4); // Create the score array
    if (scores_player_array == NULL) {
            perror("Error ");
            exit(EXIT_FAILURE);
    }
    for(int i = 0; i < number_player; i++) // Initialize scores to 0
        scores_player_array[i] = 0;

    int *moves_needed = malloc(number_player * 4); // Create the array of movements indicated by the players
    if (moves_needed == NULL) {
            perror("Error ");
            exit(EXIT_FAILURE);
    }

    for(int r = 1; r <= rounds; r++) {
        printf("Round %d\n\n", r);

        pick_robot = rdm(0, 3); // Choose the robot 
        do { pick_target = rdm(1, 16); } // Choose the target 
        while(map[array_bot[0][0]][array_bot[0][1]].target == pick_target || // check that no robot is on the chosen target
            map[array_bot[1][0]][array_bot[1][1]].target == pick_target ||
            map[array_bot[2][0]][array_bot[2][1]].target == pick_target ||
            map[array_bot[3][0]][array_bot[3][1]].target == pick_target);
        

        printf("Robot %c, target %d, Time %ds: \n", map[array_bot[pick_robot][0]][array_bot[pick_robot][1]].robot, pick_target, timer);
        // Observation time
        #ifdef _WIN32 // for Windows
        #include <windows.h>
            Sleep(timer * 1000);
        #elif defined(__linux__) // for Linux
        #include <unistd.h>
            sleep(timer);
        #endif

        // Mass display to hide the map
        for(int i = 0; i < 30; i++)
            printf("\n\n\n\n\n\n\n\n\n\n"); // Skip 10 lines, 300 lines in total
        
        printf("Time's up!\n");
        for(int i = 1; i <= number_player; i++) {
            printf("Player %d, enter your amount of movements (enter -1 if impossible):", i);
            do {
                getint(moves_needed + (i - 1));
            } while(moves_needed[i - 1] < 1 && moves_needed[i - 1] != -1);
        }
        minimum_moves = pick_min_array(moves_needed, number_player, &player_index);
        print_map(map, size_map);

        for(int i = 1; i <= minimum_moves; i++) { // if minimum_moves = -1, the loop does nothing
            do {
                printf("Robot %c, target %d. \n\n", map[array_bot[pick_robot][0]][array_bot[pick_robot][1]].robot, pick_target);
                printf("Player %d, enter movement %d (1 for up, 2 for right, 3 down and 4 left): ", player_index + 1, i);
                get_positive_int(&player_order);
            } while(player_order > 4);
    
            switch (player_order) {
            case 1:
                go_up(map, size_map, array_bot, pick_robot); // go up
                break;
            case 2:
                go_right(map, size_map, array_bot, pick_robot); // go right
                break;
            case 3:
                go_down(map, size_map, array_bot, pick_robot); // go down
                break;
            case 4:
                go_left(map, size_map, array_bot, pick_robot); // go left
                break;
            }
            
            print_map(map, size_map);
            if (map[array_bot[pick_robot][0]][array_bot[pick_robot][1]].target == pick_target) { // check if the robot is on the target
                printf("\nTarget reached!\n");
                if(i == minimum_moves) 
                    situation = 1; // the player succeeded and correctly estimated the number of movements
                else {
                    situation = 3; // the player succeeded but misestimated the number of movements
                    break;
                }
            } else if(i == minimum_moves) {
                printf("\nTarget not reached\n");
                situation = 2; // the player failed.
            }

            printf("\nMoves remaining: %d\n\n", minimum_moves - i);
        }

        // Assign points
        if (minimum_moves == -1) {
            printf("No one found a way. Round cancelled. No points attributed\n");
            r--;
        } else if (situation == 1)
            scores_player_array[player_index] += 2;
        else if (situation == 2) {
            for (int i = 0; i < number_player; i++) { // give 1 point to each other player
                if (i != player_index) // ensure not to give points to the losing player
                    scores_player_array[i]++;
            }
        } else // situation = 3
            scores_player_array[player_index]--;

        print_scores(scores_player_array, number_player); // Display scores

    }

    int max_score = scores_player_array[0];
    int winner = 0;
    for (int i = 1; i < number_player; i++) { // Determine which player has the highest score
        if (max_score < scores_player_array[i]) {
            max_score = scores_player_array[i];
            winner = i;
        }
    }

    printf("\nGame ended\n\n");
    print_winner(scores_player_array, number_player);

    for (int i = 0; i < size_map; i++) {
        free(map[i]);
    }
    free(map);

}
