#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/tile.h"
#include "include/map.h"
#include "include/game.h"
#include "include/utils.h"


void main(){

	srand(time(NULL));
    
    // Demandes de valeurs aux utilisateurs, code robuste s'assure que des valeurs cohérentes soient entrées
    int number_player;
    printf("Please enter a number of player : ");
    get_positive_int(&number_player);
    
    int rounds; 
    printf("Please enter a number of rounds : ");
    get_positive_int(&rounds);
    printf("%d Players, %d rounds\n",number_player, rounds);

    int difficulty_level, timer;
    do{
        printf("Please enter a difficulty level (1 to 4): ");
        get_positive_int(&difficulty_level);
    }
    while(difficulty_level>4);
    
    printf("\n");
    if (difficulty_level==1)
        timer = 40; // en seconde
    else if (difficulty_level==2)
        timer = 30; // en seconde
    else if (difficulty_level==3)
        timer = 20; // en seconde
    else
        timer = 10; // en seconde
    

    int size_map = rdm(15,20); // Choix aléatoire de la taille du plateau de jeu
    Tile **map = malloc(size_map * sizeof(Tile *)); // tableau 2D de case, on alloue la place pour les lignes
    // contient des pointeurs de Tiles
    if (map == NULL) {
        perror("Error \n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size_map; i++) {
        map[i] = malloc(size_map *sizeof(Tile));// à chaque ligne on alloue la place pour les collonnes
        if (map[i] == NULL) {
            perror("Error \n");
            exit(EXIT_FAILURE);
        }
    }


    int array_bot[4][2]; // tableau contenant les coordonnées de chaque robot

    //Crétion du plateau
    create_map(map,size_map);
    add_map_side_walls(map,size_map);
    add_map_target(map,size_map);
    add_map_robot(map,size_map, array_bot);
    print_map(map,size_map);

    
    int pick_robot, pick_target, player_index, minimun_moves, player_order, situation;

    int *scores_player_array = malloc(number_player*4); // Création du tableau des scores
    if (scores_player_array == NULL) {
            perror("Error ");
            exit(EXIT_FAILURE);
        }
    for(int i=0; i<number_player;i++) // Initiatlise les scores à 0
        scores_player_array[i] = 0;

    int *moves_needed = malloc(number_player*4); // Création du tableau des nombres de déplacements indiqués par les joueurs
    if (moves_needed == NULL) {
            perror("Error ");
            exit(EXIT_FAILURE);
        }

    for(int r=1; r<=rounds;r++){
        printf("Round %d\n\n",r);

        pick_robot = rdm(0,3); // Choix du robot 
        do{pick_target = rdm(1,16);} // Choix de la cible 
        while(map[array_bot[0][0]][array_bot[0][1]].target == pick_target || // vérifie qu'il n'y ait aucun robot sur la cible choisi
            map[array_bot[1][0]][array_bot[1][1]].target == pick_target ||
            map[array_bot[2][0]][array_bot[2][1]].target == pick_target ||
            map[array_bot[3][0]][array_bot[3][1]].target == pick_target);
        

        printf("Robot %c, target %d, Time %ds: \n",map[array_bot[pick_robot][0]][array_bot[pick_robot][1]].robot,pick_target,timer);
        // Temps d'observation
        #ifdef _WIN32 // pour Windows
        #include <windows.h>
            Sleep(timer*1000);
        #elif defined(__linux__)// pour linux
        #include <unistd.h>
            sleep(timer);
        #endif

        // Affichage de masse pour cacher la map
        for(int i=0;i<30;i++)
            printf("\n\n\n\n\n\n\n\n\n\n"); // Saute 10 lignes, 300 lignes au total
        
        printf("Time's up !\n");
        for(int i = 1; i<=number_player;i++){
            printf("Player %d, enter your amount of movements (enter -1 if impossible):",i);
            do{
            getint(moves_needed+(i-1));
            }
            while(moves_needed[i-1]<1 && moves_needed[i-1] !=-1);
        }
        minimun_moves = pick_min_array(moves_needed,number_player,&player_index);
        print_map(map,size_map);

        for(int i=1;i<=minimun_moves;i++){ // si minimum_moves = -1, la boucle ne fait rien
            do{
            printf("Robot %c, target %d. \n\n",map[array_bot[pick_robot][0]][array_bot[pick_robot][1]].robot,pick_target);
            printf("Player %d, enter movement %d (1 for up, 2 for right, 3 down and 4 left) : ",player_index + 1,i);
            get_positive_int(&player_order);
            }
            while(player_order>4);
    
            switch (player_order){
            case 1:
                go_up(map,size_map,array_bot,pick_robot); // aller à gauche
                break;
            case 2:
                go_right(map,size_map,array_bot,pick_robot); // aller à droite
                break;
            case 3:
                go_down(map,size_map,array_bot,pick_robot); // aller en bas
                break;
            case 4:
                go_left(map,size_map,array_bot,pick_robot); // aller à droite
                break;
            }
            
            print_map(map,size_map);
            if (map[array_bot[pick_robot][0]][array_bot[pick_robot][1]].target == pick_target){ // regarde si le robot est sur la cible
                printf("\nTarget reached !\n");
                if(i==minimun_moves) 
                    situation = 1; // le joueur a réussi et bien estimé son nombre de mouvements
                else{
                    situation = 3; // le joueur a réussi mais mal estimé son nombre de mouvements
                    break;
                }
            }
            else if(i==minimun_moves){
                printf("\nTarget not reached\n");
                situation = 2;// le joueur a échoué.
            }

            printf("\nMoves remaining : %d\n\n",minimun_moves-i);
        }

        // Attribution des points
        if (minimun_moves==-1){
            printf("No one find a way. Round cancelled. No points attributed\n");
            r--;
        }
        else if (situation==1)
            scores_player_array[player_index] += 2;
        else if (situation==2){
            for (int i = 0; i<number_player;i++){ // on donne 1 point à chaque autres joueurs
                if (i != player_index) // on veille à ne pas donner de point au joueur perdant
                    scores_player_array[i] ++;
            }
        }
        else // situation = 3
            scores_player_array[player_index] --;
        
        print_scores(scores_player_array, number_player); // Affichage des scores

    }

    int max_score = scores_player_array[0];
    int winner = 0;
    for (int i = 1; i<number_player;i++){ // Détermine quel joueur a le plus haut score
        if (max_score<scores_player_array[i]){
            max_score = scores_player_array[i];
        winner = i;
        }
    }

    printf("\nGame ended \n\n");
    print_winner(scores_player_array,number_player);

    for (int i=0; i<size_map;i++){
        free(map[i]);
    }
    free(map);

}
