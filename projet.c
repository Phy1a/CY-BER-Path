#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


typedef struct
{
    int upper_wall;
    int lower_wall;
    int right_wall;
    int left_wall;

    int target;
    char robot;
}Tile;


Tile create_tile(){
    Tile a;

    // 0 = pas de mur, 1 = mur et -1 = même mur que la case adjacente
    // à l'initilisation d'une case, celle-ci n'a as de mur
    a.upper_wall = 0;  
    a.lower_wall = 0; 
    a.right_wall = 0;
    a.left_wall = 0; 

    a.robot = 64;

    a.target = 0;

    return a;
}

void better_tile(Tile *a, int row, int column){/* Cette fonction formate les cases pour un affichage épurer, elle attribue des valeurs -1 quand le mur 
est déjà matérialiser par le mur de la case d'à côté, sans cette fonction on aurait des affichages comme ceci : Case A || Case B 
avec 2 murs côte à côte*/
    if (row){
        a->upper_wall = -1;// la valeur -1 dit à l'affichage de ne rien afficher
    }
    if(column){
        a->left_wall = -1; 
    }
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

int rdm(int mini, int maxi){ // génère un entier aléatoire entre min et max inclus
    return rand() % (maxi - mini + 1) + mini;
}


void create_map(Tile **map, int size_map){
    Tile a;

    for (int row = 0; row<size_map;row++){
        for (int column = 0; column<size_map; column++){
            a = create_tile();
            better_tile(&a, row, column);
            map[row][column]= a;
        }
    }
    
    for (int column = 0; column<size_map; column++)
            map[0][column].upper_wall=1;

    for (int column = 0; column<size_map; column++)
            map[size_map-1][column].lower_wall=1;
    
    for (int row = 0; row<size_map; row++)
            map[row][0].left_wall=1;

    for (int row = 0; row<size_map; row++)
            map[row][size_map-1].right_wall=1;
}


void print_tile1(Tile a){// affiche la première ligne de la case a
    if (a.upper_wall==1)
        printf("-----");
    if (a.upper_wall==0)
        printf(".....");
}

void print_tile2(Tile a){ // affiche la deuxième ligne de la case a

    if (a.left_wall==1)
        printf("| ");
    else if(a.left_wall==0)
        printf(": ");
    else if (a.left_wall==-1)
        printf(" ");
    // si a.left_wall==-1 on n'affiche rien
        
    
    if(a.robot != 64){
        printf(" %c",a.robot);
    }
    else if (a.target){
        if (a.target<10)
            printf(" ");
        printf("%d",a.target);
    }
    else{
        printf("  ");
    }

    if (a.right_wall==1)
        printf(" |");
    else if(a.right_wall==-1)
        printf(" ");
    else if(a.right_wall==0)
        printf(" :");
}

void print_tile3(Tile a){ // affiche la troisième ligne de la case a
    if (a.lower_wall==1)
        printf("-----");
    else if (a.lower_wall==0)
        printf(".....");
    // si a.lower_wall==-1 on n'affiche rien 
}


void print_row(Tile **map,int row,int size_map){ // affiche la ligne row de la grille map de taille size_map
    for(int column=0; column<size_map;column++)
        print_tile1(map[row][column]);
       
	if (map[row][0].upper_wall != -1)/*si le premier mur vaut -1, alors toute la ligne de murs vaut -1, cf la fonction set_row*/
		(map[row][size_map-1].upper_wall) ? printf("-\n") : printf(".\n");


    for(int column=0; column<size_map;column++){
        print_tile2(map[row][column]);
    }
    printf("\n");
    
    for(int column=0; column<size_map;column++){
        print_tile3(map[row][column]);
    }
    if (map[row][size_map-1].lower_wall==1)
        printf("-");
    else if (map[row][size_map-1].lower_wall==0)
        printf(".");
    printf("\n");
}

void print_map(Tile **map, int size_map){
    for (int row=0; row<size_map;row++){
        print_row(map,row,size_map);
        }
    printf("\n");
}


int is_wall(Tile **map, int size_map, int row, int column, int position){/* renvoie 1 s'il y a déjà un mur, 0 sinon */
	switch (position){
		case 1: // Mur du haut
			if(map[row][column].upper_wall == -1) // on vérifie si la case du dessus a un mur en bas
				return (map[row-1][column].lower_wall == 1) ? 1 : 0;
			else
				return (map[row][column].upper_wall == 1) ? 1 : 0;
			break;
			
		case 2: // Mur de droite
			return (map[row][column].right_wall == 1) ? 1 : 0;
			break;

		case 3: // Mur du bas
			return (map[row][column].lower_wall == 1) ? 1 : 0;
			break;

		case 4: // Mur de gauche
			if(map[row][column].left_wall == -1)
				return (map[row][column-1].right_wall == 1) ? 1 : 0; //on vérifie si la case de gauche a un mur à droite
			else
				return (map[row][column].left_wall == 1) ? 1 : 0;
			break;	
	}
}

void add_wall(Tile **map, int size_map, int row, int column, int position){/* ajoute le mur à la case souhaitée 
et dans la position souhaité (haut, droite, bas , gauche) = position(1,2,3,4)
Ajuste aussi le mur de la case voisine*/
	if (!is_wall(map,size_map,row, column, position)){ // on vérifie qu'il n'y ait pas déjà un mur
		switch (position){
			case 1: // Ajout mur du haut
				(map[row][column].upper_wall==-1) ? (map[row-1][column].lower_wall= 1) : (map[row][column].upper_wall = 1);
				break;
			case 2: // Ajout mur de droite
				(map[row][column]).right_wall = 1;
				break;
			case 3: // Ajout mur du bas
				(map[row][column]).lower_wall = 1;
				break;
			case 4: // Ajout mur de gauche
				(map[row][column].left_wall==-1) ? (map[row][column-1].right_wall= 1) : (map[row][column].left_wall = 1);
				break;
		}
	}
}


int is_target_valid(Tile **map,int size_map, int row, int column){/* Regarde si l'emplacement donnée est éligible ou non
Retourne 0 (non valide) ou 1 (valide) */

    //vérifie que les cibles ne soient pas en contact avec les murs aléatoires des bordures afin d'éviter tout problème
    if(row==1){
        if(is_wall(map,size_map,0,column,2) || is_wall(map,size_map,0,column,4))
            return 0;
    }
    else if(row==size_map-2){
        if(is_wall(map,size_map,size_map-1,column,2) || is_wall(map,size_map,size_map-1,column,4))
            return 0;
    }
    if(column==1){
        if(is_wall(map,size_map,row,0,1) || is_wall(map,size_map,row,0,3))
            return 0;
    }
    else if(column==size_map-2){
        if(is_wall(map,size_map,row,size_map-1,1) || is_wall(map,size_map,row,size_map-1,3))
            return 0;
    }
    

    if(map[row][column].target)// vérifie si la case choisi ne contient pas déjà une cible
        return 0;

    // vérifie si les cases du dessus contiennent des cibles
    if(map[row-1][column-1].target || map[row-1][column].target || map[row-1][column+1].target)
        return 0;

    // vérifie si les cases du dessous contiennent des cibles
    if(map[row+1][column-1].target || map[row+1][column].target || map[row+1][column+1].target)
        return 0;
    
    // vérifie si les cases d'à côté contiennent des cibles
    if(map[row][column-1].target || map[row][column+1].target)
        return 0;
    
    return 1; //sinon retourne 1 pour vrai
}


void  random_wall(int *array,int size_map){//remplie un tableau avec des coordonnées alétoire pour des murs
    do{
        array[0] = rdm(2,size_map-2);
        array[1]= rdm(2,size_map-2);
    }
    while(abs(array[0]-array[1])<2); // deux murs côté à côte sont inutilisables pour les joueurs
}

void random_target(Tile **map, int size_map, int* x, int* y){ // donne les coordonnées aléatoire valide d'une cible
    do{
    *x = rdm(1,size_map-2);
    *y = rdm(1,size_map-2);
    }   while(!is_target_valid(map,size_map,*x,*y)); // on vérifie que les coordonnées soient valides
}

void random_robot(Tile **map, int size_map, int* x, int* y){ // donne les coordonnées aléatoire valide d'un robot
    do{
    *x = rdm(0,size_map-1);
    *y = rdm(0,size_map-1);
    }   while(map[*x][*y].target); //vérifie que le robot ne soit pas sur une cible
}


void add_map_side_walls(Tile **map, int size_map){
    int array[2] = {0}; // tableau des coordonées du mur de chaque côté du plateau

    random_wall(array,size_map); // on change les murs aléatoires pour chaque côté de la grille donc 4 fois
    for(int i=0;i<2;i++)
        add_wall(map,size_map,0,array[i],4); // ajouts des 2 murs du haut vers la gauche

    random_wall(array,size_map); 
    for(int i=0;i<2;i++)
        add_wall(map,size_map,size_map-1,array[i],4); // ajouts des 2 murs du bas vers la gauche
    
    random_wall(array,size_map);
    for(int i=0;i<2;i++)
        add_wall(map,size_map,array[i],0,1); // ajouts des 2 murs de gauche vers le haut

    random_wall(array,size_map);
    for(int i=0;i<2;i++)
        add_wall(map,size_map,array[i],size_map-1,1); // ajouts des 2 murs de droite vers le haut
}

void add_map_target(Tile **map, int size_map){
    int row,column,a;
    for (int n=1;n<17;n++){ 
        random_target(map,size_map,&row,&column); // Génère des coordonnées de cibles valide
        map[row][column].target = n; // on les ajoute au plateau
        
        a = rdm(1,4);
        add_wall(map,size_map,row,column,a); // 1er mur autour de la cible aléatoire (=a)
        add_wall(map,size_map,row,column,a%4+1); // 2ème mur qui est dans la position suivante
    }
}

void add_map_robot(Tile **map, int size_map, int array[4][2]){
    int row,column;
    for (int n=0;n<4;n++){ 
        random_robot(map,size_map,&row,&column);
        array[n][0]=row;
        array[n][1]=column;
        map[row][column].robot = n+65;
    }
}


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


int pick_min_array(int *array,int size_array,int *player_index){// Selection au hasard le joueur qui jouera parmi ceux à égalité

    int mini = array[0];
    *player_index = 0;
    for (int i=1; i<size_array; i++){// on parcourt une première fois le tableau pour trouver le minimum

        if (mini==-1 && array[i]>mini){ // dans le cas où array[0] = -1;
             mini = array[i];
            *player_index = i;
        }

        if (array[i]<mini && array[i] != -1){ // -1 veut dire que le joueur n'a pas trouvé de solutions
            mini = array[i];
            *player_index = i;
        }
    }
    
    int count = 0; // Nombre d'égalité
    for (int i=0; i<size_array; i++){// une deuxième fois pour compter les potentielles égalités
        if (array[i]==mini)
            count ++;
    }

    int roll_dices = rdm(1,count); // choix aléatoire du joueurs choisi
    int i =0;
    while(count>1){ // une dernière fois pour renvoyer le joueur choisis au hasard parmi les églités
        if (array[i]==mini){
            if (roll_dices==1){
                *player_index = i;
                break;
            }
            roll_dices--;
            count--;
        }
        i++;  
    }

    return mini;
}

void print_scores(int *array, int size_array){ // Affiche le tableau des scores
    for (int i=0; i<size_array;i++){
        printf("Player %d : %d point",i+1,array[i]);
        if (array[i]>1)
            printf("s");
        printf("\n");
    }
}

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
        printf("Please enter a difficulty level (1 to 3): ");
        get_positive_int(&difficulty_level);
    }
    while(difficulty_level>3);
    
    printf("\n");
    if (difficulty_level==1)
        timer = 60; // en seconde
    else if (difficulty_level==2)
        timer = 40; // en seconde
    else
        timer = 20; // en seconde
    

    int size_map = rdm(15,20); // Choix aléatoire de la taille du plateau de jeu
    Tile **map = malloc(size_map*8); // tableau 2D de case, on alloue la place pour les lignes
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

        printf("Round %d\n",r);

        pick_robot = rdm(0,3); // Choix du robot 
        do{pick_target = rdm(1,16);} // Choix de la cible 
        while(map[array_bot[0][0]][array_bot[0][1]].target == pick_target || // vérifie qu'il n'y ait aucun robot sur la cible choisi
            map[array_bot[1][0]][array_bot[1][1]].target == pick_target ||
            map[array_bot[2][0]][array_bot[2][1]].target == pick_target ||
            map[array_bot[3][0]][array_bot[3][1]].target == pick_target);
        

        printf("Robot %c, target %d, Time %ds: \n",map[array_bot[pick_robot][0]][array_bot[pick_robot][1]].robot,pick_target,timer);
        Sleep(timer*1000);// Temps d'observation

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

        for(int i=1;i<=minimun_moves;i++){
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
            printf("\nMoves remaining : %d\n\n",minimun_moves-i);
        }
        if (map[array_bot[pick_robot][0]][array_bot[pick_robot][1]].target != pick_target) // si le robot n'est pas sur la 
        //cible après que tous les mouvements ont été épuisé
            situation = 2;// le joueur a échoué.

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
        else if (situation == 3)
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

    printf("\n Game ended \n\n Winner is Player %d\n\n",winner+1);


    for (int i=0; i<size_map;i++){
        free(map[i]);
    }
    free(map);

}

/*  mettre la map en couleur (custom couleurs pour cibles et robots) */
// purge
