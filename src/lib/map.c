#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/map.h"

void create_map(Tile **map, int size_map){
    Tile a;

    for (int row = 0; row<size_map;row++){
        for (int column = 0; column<size_map; column++){
            a = create_tile();
            better_tile(&a, row, column);
            map[row][column]= a;
        }
    }
    
    for (int column = 0; column<size_map; column++) //ajoute les murs du haut de la première ligne
            map[0][column].upper_wall=1;

    for (int column = 0; column<size_map; column++) // murs du bas de la dernière ligne
            map[size_map-1][column].lower_wall=1;
    
    for (int row = 0; row<size_map; row++) // murs de gauche de la première colonne
            map[row][0].left_wall=1;

    for (int row = 0; row<size_map; row++) // murs de droite de la dernnière colonne
            map[row][size_map-1].right_wall=1;
}


void print_row(Tile **map,int row,int size_map){ // affiche la ligne row de la grille map de taille size_map
    for(int column=0; column<size_map;column++)
        print_tile1(map[row][column]);
    
    // Corection visuelle
	if (map[row][0].upper_wall != -1)//si le premier mur vaut -1, alors toute la ligne de murs vaut -1
		(map[row][size_map-1].upper_wall) ? print_colored("-\n", BLUE) : printf(" \n");


    for(int column=0; column<size_map;column++){
        print_tile2(map[row][column]);
    }
    printf("\n");
    
    for(int column=0; column<size_map;column++){
        print_tile3(map[row][column]);
    }

    // Corection visuelle
    if (map[row][size_map-1].lower_wall==1)
        print_colored("-", BLUE);
    else if (map[row][size_map-1].lower_wall==0)
        printf(" ");

    printf("\n");
}


void print_map(Tile **map, int size_map){
    for (int row=0; row<size_map;row++){ // Affiche toutes les lignes
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

    // vérifie si les cases du dessus ne contiennent pas de cibles
    if(map[row-1][column-1].target || map[row-1][column].target || map[row-1][column+1].target)
        return 0;

    // vérifie si les cases du dessous ne contiennent pas de cibles
    if(map[row+1][column-1].target || map[row+1][column].target || map[row+1][column+1].target)
        return 0;
    
    // vérifie si les cases d'à côté ne contiennent pas de cibles
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
    for (int n=1;n<17;n++){ // Génère des coordonnées de cibles valides
        do { 
        row = rdm(1,size_map-2);
        column = rdm(1,size_map-2);
        } while (!is_target_valid(map, size_map, row, column)); 

        map[row][column].target = n; // on les ajoute au plateau
        
        a = rdm(1,4);
        add_wall(map,size_map,row,column,a); // 1er mur autour de la cible aléatoire (=a)
        add_wall(map,size_map,row,column,a%4+1); // 2ème mur qui est dans la position suivante
    }
}


void add_map_robot(Tile **map, int size_map, int array[4][2]){
    int row,column;
    for (int i=0;i<4;i++){ // Génère des coordonnées de robots valides
        do { 
            row = rdm(0,size_map-1);
            column = rdm(0,size_map-1);
        } while (map[row][column].robot != 64);
        array[i][0]=row;
        array[i][1]=column;
        map[row][column].robot = i+65;
    }
}