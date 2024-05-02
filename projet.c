#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


typedef struct
{
    int coords_x;
    int coords_y;

    int upper_wall;
    int lower_wall;
    int right_wall;
    int left_wall;

    int target;
    char robot;
}Tile;


Tile create_tile(int row, int column){
    Tile a;

    // les coordonnées varient entre 0 et size_map - 1
    a.coords_x = row; 
    a.coords_y = column;

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

void better_tile(Tile *a){/* Cette fonction formate les cases pour un affichage épurer, elle attribue des valeurs -1 quand le mur 
est déjà matérialiser par le mur de la case d'à côté, sans cette fonction on aurait des affichages comme ceci : Case A || Case B 
avec 2 murs côte à côte*/
    int i = a->coords_x;
    int j = a->coords_y;
    if (i){
        a->upper_wall = -1;// la valeur -1 dit à l'affichage de ne rien afficher
    }
    if(j){
        a->left_wall = -1; 
    }
}


void getint(int *result){ /* fonction qui demande un nombre strictement positif 
à l'utilisateur et s'assure de la validité de la réponse*/
    int error_detector = scanf("%d",result);
    printf("%d\n",error_detector);
    while (!error_detector || *result<=0){
        printf("Please enter a realistic number of player : ");
        if(!error_detector){
            while(getchar() !='\n');  // enlève tous les caractères bloqués dans le tamponn car inutilisé par scanf
        //pour qu'on puisse reentrer une nouvelle valeur dans number_player, sans cela le programme boucle
        }
        error_detector = scanf("%d",result);
    }
}

int rdm(int mini, int maxi){ // génère un int aléatoire entre min et max inclus
    return rand() % (maxi - mini + 1) + mini;
}


void create_map(Tile **map, int size_map){
    Tile a;

    for (int i = 0; i<size_map;i++){
        for (int j = 0; j<size_map; j++){
            a = create_tile(i,j);
            better_tile(&a);
            map[i][j]= a;
        }
    }
    
    for (int j = 0; j<size_map; j++)
            map[0][j].upper_wall=1;

    for (int j = 0; j<size_map; j++)
            map[size_map-1][j].lower_wall=1;
    
    for (int i = 0; i<size_map; i++)
            map[i][0].left_wall=1;

    for (int i = 0; i<size_map; i++)
            map[i][size_map-1].right_wall=1;
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
    else if(a.left_wall==-1)
        printf(" ");
    else // a.left_wall==0
        printf(": ");
    
    if(a.robot != 64){
        printf("%c ",a.robot);
    }
    else if (a.target != 0){
        printf("%d",a.target);
        if (a.target<10)
            printf(" ");
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
    else if (a.lower_wall==-1)
        printf("");
    else // a.lower_wall==0 
        printf(".....");
}

void print_tile(Tile a){// fonction de test, inutile dans le programme
    print_tile1(a);
    printf("\n");
    print_tile2(a);
    printf("\n");
    print_tile3(a);
}


void print_row(Tile **map,int row,int size_map){ // affiche la ligne row de la grille map de taille size_map
    for(int j=0; j<size_map;j++)
        print_tile1(map[row][j]);
       
	if (map[row][0].upper_wall != -1)/*si le premier mur vaut -1, alors toute la ligne de murs vaut -1, cf la fonction set_row*/
		(map[row][size_map-1].upper_wall) ? printf("-\n") : printf(".\n");


    for(int j=0; j<size_map;j++){
        print_tile2(map[row][j]);
    }
    printf("\n");
    
    for(int j=0; j<size_map;j++){
        print_tile3(map[row][j]);
    }
    if (map[row][size_map-1].lower_wall==1)
        printf("-");
    else if (map[row][size_map-1].lower_wall==0)
        printf(".");
    printf("\n");
}

void print_map(Tile **map, int size_map){
    for (int i=0; i<size_map;i++){
        print_row(map,i,size_map);
        }
    printf("\n");
}



int is_wall(Tile **map, int size_map, int row, int column, int position){/* renvoie 1 s'il y a déjà un mur, 0 sinon */
	switch (position){
		case 1:
			if(map[row][column].upper_wall == -1)
				return (map[row-1][column].lower_wall == 1) ? 1 : 0;
			else
				return (map[row][column].upper_wall == 1) ? 1 : 0;
			break;
			
		case 2:
			return (map[row][column].right_wall == 1) ? 1 : 0;
			break;

		case 3:
			return (map[row][column].lower_wall == 1) ? 1 : 0;
			break;

		case 4:
			if(map[row][column].left_wall == -1)
				return (map[row][column-1].right_wall == 1) ? 1 : 0;
			else
				return (map[row][column].left_wall == 1) ? 1 : 0;
			break;	
	}
}


void add_wall(Tile **map, int size_map, int row, int column, int position){/* ajoute le mur à la case souhaitée 
et dans la position souhaité (haut, droite, bas , gauche) = position(1,2,3,4)
Ajuste aussi le mur de la case voisine*/
	if (!is_wall(map,size_map,row, column, position)){
		switch (position){
			case 1:
				(map[row][column].upper_wall==-1) ? (map[row-1][column].lower_wall= 1) : (map[row][column].upper_wall = 1);
				break;
			case 2:
				(map[row][column]).right_wall = 1;
				break;
			case 3:
				(map[row][column]).lower_wall = 1;
				break;
			case 4:
				(map[row][column].left_wall==-1) ? (map[row][column-1].right_wall= 1) : (map[row][column].left_wall = 1);
				break;
		}
	}
}

//Cette fonction n'est pas censée exister, on n'est pas censer avoir besoin de retirer des murs
void remove_wall(Tile **map, int size_map, int row, int column, int position){/* retire le mur à la case souhaitée 
et dans la position souhaité (haut, droite, bas , gauche) = position(1,2,3,4)
Ajuste aussi le mur de la case voisine.*/

	if (is_wall(map,size_map,row, column, position)){
		switch (position){
			case 1:
				(map[row][column].upper_wall==-1) ? (map[row-1][column].lower_wall= 0) : (map[row][column].upper_wall = 0);
				break;
			case 2:
				(map[row][column]).right_wall = 0;
				break;
			case 3:
				(map[row][column]).lower_wall = 0;
				break;
			case 4:
				(map[row][column].left_wall==-1) ? (map[row][column-1].right_wall= 0) : (map[row][column].left_wall = 0);
				break;
		}
	}
}


int is_target_valid(Tile **map,int size_map, int row, int column){
    
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


void  random_wall(int *tab,int size_map){//remplie un tableau avec des coordonnées alétoire pour des murs
    if (size_map>3)//Les 2 if sont uniquement pour des tests sur des maps de size < 15
        tab[0] = rdm(2,size_map-3);
    if(size_map>5){
        do{
            tab[0] = rdm(2,size_map-2);
            tab[1]= rdm(2,size_map-2);
        }
        while(abs(tab[0]-tab[1])<2);
    }

}

void random_target(Tile **map, int size_map, int* x, int* y){ // donne les coordonnées aléatoire d'une cible
    do{
    *x = rdm(1,size_map-2);
    *y = rdm(1,size_map-2);
    }   while(!is_target_valid(map,size_map,*x,*y));
}

void random_robot(Tile **map, int size_map, int* x, int* y){ // donne les coordonnées aléatoire valide d'un robot
    do{
    *x = rdm(0,size_map-1);
    *y = rdm(0,size_map-1);
    }   while(map[*x][*y].target);//vérifie que les cibles ne soient pas sur une cible
}


void add_map_side_walls(Tile **map, int size_map){
    int tab[2] = {0};

    random_wall(tab,size_map);
    for(int i=0;i<2;i++)// murs du haut vers la gauche
        add_wall(map,size_map,0,tab[i],4);

    random_wall(tab,size_map); // on change les murs aléatoires pour chaque côté de la grille
    for(int i=0;i<2;i++)// murs du bas vers la gauche
        add_wall(map,size_map,size_map-1,tab[i],4);
    
    random_wall(tab,size_map);
    for(int i=0;i<2;i++)// murs de gauche vers le haut
        add_wall(map,size_map,tab[i],0,1);

    random_wall(tab,size_map);
    for(int i=0;i<2;i++)// murs de droite vers le haut
        add_wall(map,size_map,tab[i],size_map-1,1);
}

void add_map_target(Tile **map, int size_map){
    int row,column,a;
    for (int n=1;n<17;n++){ 
        random_target(map,size_map,&row,&column);
        map[row][column].target = n;
        a = rdm(1,4);

        // Il y avait des murs additionnels qui apparaissaient de maières aléatoire autours de certaines cibles
        // Ces 4 lignes de codes ne devraient même pas exister mais sont là uniquement paour corriger ce "bug"
        // Donc elles retirent les murs autour de la cibles créée.
        remove_wall(map,size_map,row,column,1);
        remove_wall(map,size_map,row,column,2);
        remove_wall(map,size_map,row,column,3);
        remove_wall(map,size_map,row,column,4);

        add_wall(map,size_map,row,column,a); // 1 er mur autour de la cible aléatoire (=a)
        add_wall(map,size_map,row,column,a%4+1); // 2ème mur qui est dans la position suivante
    }
}

void add_map_robot(Tile **map, int size_map, int tab[4][2]){
    int row,column;
    for (int n=0;n<4;n++){ 
        random_robot(map,size_map,&row,&column);
        tab[n][0]=row;
        tab[n][1]=column;
        map[row][column].robot = n+65;
    }
}


int go_up (Tile **map, int size_map,int tab[4][2], int bot, int distance){
    if (is_wall(map,size_map,tab[bot][0],tab[bot][1],1))
        return distance;
    if (map[tab[bot][0]][tab[bot][1]+1].robot != 64)
        return distance;

    map[tab[bot][0]-1][tab[bot][1]].robot = map[tab[bot][0]][tab[bot][1]].robot;
    map[tab[bot][0]][tab[bot][1]].robot = 64;
    tab[bot][0] -= 1;

    return go_up(map,size_map,tab,bot,distance+1);
}

int go_right (Tile **map, int size_map,int tab[4][2], int bot, int distance){
    if (is_wall(map,size_map,tab[bot][0],tab[bot][1],2))
        return distance;
    if (map[tab[bot][0]][tab[bot][1]+1].robot != 64)
        return distance;

    map[tab[bot][0]][tab[bot][1]+1].robot = map[tab[bot][0]][tab[bot][1]].robot;
    map[tab[bot][0]][tab[bot][1]].robot = 64;
    tab[bot][1] += 1;

    return go_right(map,size_map,tab,bot,distance+1);
}

int go_down (Tile **map, int size_map,int tab[4][2], int bot, int distance){
    if (is_wall(map,size_map,tab[bot][0],tab[bot][1],3))
        return distance;
    if (map[tab[bot][0]][tab[bot][1]+1].robot != 64)
        return distance;

    map[tab[bot][0]+1][tab[bot][1]].robot = map[tab[bot][0]][tab[bot][1]].robot;
    map[tab[bot][0]][tab[bot][1]].robot = 64;
    tab[bot][0] += 1;

    return go_down(map,size_map,tab,bot,distance+1);
}

int go_left (Tile **map, int size_map,int tab[4][2], int bot, int distance){
    if (is_wall(map,size_map,tab[bot][0],tab[bot][1],4))
        return distance;
    if (map[tab[bot][0]][tab[bot][1]+1].robot != 64)
        return distance;

    map[tab[bot][0]][tab[bot][1]-1].robot = map[tab[bot][0]][tab[bot][1]].robot;
    map[tab[bot][0]][tab[bot][1]].robot = 64;
    tab[bot][1] -= 1;

    return go_left(map,size_map,tab,bot,distance+1);
}

void main(){

	srand(time(NULL));
    
    /*
    int number_player;
    printf("Please enter a number of player : ");
    getint(&number_player);
    
    int rounds; // on refait la même chose pour rounds
    printf("Please enter a number of rounds : ");
    getint(&rounds);
    printf("%d Players, %d rounds\n",number_player, rounds);
    */

    
    int size_map = rdm(15,20);
    size_map = 15;
    Tile **map = malloc(size_map*8); // tableau 2D de case, on alloue la place pour les lignes
    // contient des pointeurs de Tiles
    if (map == NULL) {
        printf("Error, can't allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size_map; i++) {
        map[i] = malloc(size_map *sizeof(Tile));// à chaque ligne on alloue la place pour les collonnes
        if (map[i] == NULL) {
            printf("Error, can't allocate memory.\n");
            exit(EXIT_FAILURE);
        }
    }

    create_map(map,size_map);

    map[0][0].target = size_map;
    int tab_bot[4][2]; // tableau contenant les coordonnées de chaque robot

    add_map_side_walls(map,size_map);
    add_map_target(map,size_map);
    add_map_robot(map,size_map, tab_bot);
    
    print_map(map,size_map);
    
    for (int i=0; i<size_map;i++){
        free(map[i]);
    }
    free(map);

}

/*  mettre la map en couleur (custom couleurs pour cibles et robots) */
// modifier getint et remplacer les scan f dedans pour fixe le bug 5sdhf
