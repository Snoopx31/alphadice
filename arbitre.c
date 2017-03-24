#include "map.h"
#include "arbitre.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>

/*
Fonction vérifiant un tour rendu
Renvoie 1 si le tour est bon, -1 si il est pas valide
*/
void verify(unsigned int i, SMap *map,STurn *turn){
	if(verifyTurn(i, map, turn)){
		printf("Tour validé ! \n");
		moveTurn(map, turn);
		//SDL_Delay(500);
	}
	else{
		printf("Tour non validé ! \n");
	}
}
int verifyTurn(unsigned int idPlayer, SMap *map, STurn *turn)
{
	//Verifiez que la cellule de départ est bien au joueur
	if(map->cells[turn->cellFrom].owner != idPlayer)
	{
		return -1;
	}

	if(map->cells[turn->cellFrom].nbDices <= 1)
	{
		return -1;
	}

	//Verifier que la cellule de départ et d'arrivée sont adjacentes
	for(int i = 0; i < map->cells[turn->cellFrom].nbNeighbors; i++){
		if(map->cells[turn->cellFrom].neighbors[i]->id == turn->cellTo){
			return 1;
		}
	}
	return -1;
}

/*
Simule le nombre de lancers de dé passé en paramètres et renvoie la somme des lancers
*/
int lancerDe(int nbDe){
	int s = 0;
	for(int i=0 ; i < nbDe ; i++)
	{
		s += aleatoire(1, 6);
	}
	return s;
}

/* Fonction qui jour le tour et modifie la map en conséquent */

void moveTurn(SMap *map, STurn *turn){
	SCell *cellAttacker = &map->cells[turn->cellFrom];
	SCell *cellDefender = &map->cells[turn->cellTo];
	int sommeAttacker = lancerDe(cellAttacker->nbDices);
	int sommeDefender = lancerDe(cellDefender->nbDices);
	//printf("%d vs %d\n", sommeAttacker, sommeDefender);
	if(sommeAttacker > sommeDefender){
		cellDefender->owner = cellAttacker->owner;
		cellDefender->nbDices = cellAttacker->nbDices - 1;
	}
	//printf("nb dés : %d\n", map->cells[turn->cellFrom].nbDices);
	cellAttacker->nbDices = 1;
	//printf("nb dés : %d\n", map->cells[turn->cellFrom].nbDices);

	char str2[1000];
	sprintf(str2, "%d,%d,%d,%d,%d\n",  cellAttacker->owner, turn->cellFrom, turn->cellTo, sommeAttacker, sommeDefender);
	Log(str2);
}

/*
Trouve le plus grand nombre de territoire connexe appartenant au joueur passé en paramètre
*/
int getDiceToDistribute(int idPlayer, SMap *map){
	/*colorier x avec i
	pour tout sommet y successeur de x
	  si y n'est pas colorié faire CC_sommet(G,y,i)*/

	// SCell* tabCellOwner[map->nbCells];
	// SCell* tabVoisins[map->nbCells];
	// int i = 0, y = 0, j = 0, z = 0, count = 0, x = 0, bestCount = 0, n = 0;
	// SCell* cellActive;
	// //Rempli le tableau de toutes les cellules du joueur
	// for(;i<nbCells;i++){
	// 	if (map->cells[i]->owner == idPlayer){
	// 		tabCellOwner[y]=map->cells[i];
	// 		y++;
	// 	}
	// }
	//
	// //Boucle sur la taille du tableau des cellules du joueur
	// for(i = 0; i<y; i++){
	// 	//Si la cellule existe (Et non écrasée plus tard)
	// 	if(tabCellOwner[i]!=NULL){
	// 		cellActive = tabCellOwner[i];
	// 		//Pour chaque voisin de la cellule actuelle
	// 		for(;j<cellActive->nbNeighbors;j++){
	// 			//Si le voisin actuel est au même joueur
	// 			if(cellActive->neighbors[j]->owner==idPlayer){
	// 				//On cherche cette cellule dans le tableau des cellules du joueur pour la supprimer comme on va
	// 				//vérifier plus tard ses voisins par récursivité plus en utilisant le tableau tabVoisins.
	// 				for(; z < y; y++){
	// 					if(tabCellOwner[z]==cellActive->neighbors){
	// 						tabCellOwner[z] = NULL;
	// 						tabVoisins[x] = cellActive->neighbors[j];
	// 						x++;
	// 						count += 1;
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// 	//On met à jour le count avec les recursions sur le tableau des voisins
	// 	count += recurGetDiceToDistribute(tabCellOwner, tabVoisins);
	// 	if (bestCount < count){
	// 		bestCount = count;
	// 	}
	// 	// On remet à 0 le tableau des voisins pour le prochain territoire
	// 	for(;n<x;n++){
	// 		tabVoisins[n] == NULL;
	// 	}
	//
	// }
	//return bestCount;

	int nbDices = 0;
	for(int i = 0; i < map->nbCells; i++){
		if(map->cells[i].owner == idPlayer){
			nbDices += 1;
		}
	}
	return nbDices;
}

/*
Fonction qui répartit les dés a la fin d'un tour d'un joueur
*/
void endTurn(int idPlayer, SMap *map)
{
	int playerCell[60];
	int nbPlayerCell = 0;

	//On génére un tableau contenant les position des cellules dans map->cells
	for(int i = 0 ; i < map->nbCells ; i++){
		if(map->cells[i].owner == idPlayer){
			playerCell[nbPlayerCell] = i;
			nbPlayerCell++;
		}
	}

	int nbDiceDistributed = getDiceToDistribute(idPlayer, map);
	int random;
	Log("///\n");
	//On prend un sommet aléatoire qu'il possède et on ajoute un dé
	for(int i = 1 ; i <= nbDiceDistributed ; i++){
		random = aleatoire(0, nbPlayerCell-1);
		if(map->cells[playerCell[random]].nbDices < 8){
			map->cells[playerCell[random]].nbDices++;
			char str[100];
			sprintf(str, "%d\n", map->cells[playerCell[random]].id);
			Log(str);
		}
	}
	Log("///\n");
}

// Verifie les paramètres mis par l'utilisateur au lancement du
// programme. Modifie la valeur du pointeur nbLib passé en parametre pour matcher le nombre de lib
int verifArguments(int argc, char* argv[], int *nbLib){

	if(argc<3 || argc>5){
		rappelSyntaxe("Mauvais nombre d'arguments\n");
		return 1;
	}

	int nbPlayer = (atoi(argv[2]));
	int nbGames = (atoi(argv[1]));

	if(((nbGames<1)||(nbGames>9))||((nbPlayer<2)||(nbPlayer>8))){
		rappelSyntaxe("Mauvais paramètres\n");
		return 1;
	}

	*nbLib = argc - 3;
	return 0;
}

/* Renvoie 1 si le joueur correspondant a l'id passé en parametres a gagné
	0 sinon
*/

int victoire(unsigned int idPlayer, SMap *map){
	for(int i = 0; i < map->nbCells; i++)	{
		if(map->cells[i].owner != idPlayer){
			return 0;
		}
	}
	return 1;
}

/*
	Fonction permettant de renvoyer une copie profonde de la map
*/
SMap* deepCopy(SMap *map){
	SMap* mapCopy = malloc(sizeof(SMap));

	if(mapCopy == NULL)
		exit(-1);

	mapCopy->nbCells = map->nbCells;
	mapCopy->cells = malloc(mapCopy->nbCells * sizeof(SCell));

	if(mapCopy->cells == NULL)
		exit(-1);

	for(int i=0 ; i < mapCopy->nbCells; i++){
		mapCopy->cells[i].id = map->cells[i].id;
		mapCopy->cells[i].owner = map->cells[i].owner;
		mapCopy->cells[i].nbDices = map->cells[i].nbDices;
		mapCopy->cells[i].nbNeighbors = map->cells[i].nbNeighbors;
		mapCopy->cells[i].neighbors = malloc(mapCopy->cells[i].nbNeighbors * sizeof(SCell*));

		if(mapCopy->cells[i].neighbors == NULL)
			exit(-1);
	}

	for(int i=0 ; i < mapCopy->nbCells; i++){
		for(int j=0 ; j  < mapCopy->cells[i].nbNeighbors; j++){
			int idToAdd = map->cells[i].neighbors[j]->id;
			mapCopy->cells[i].neighbors[j] = &(mapCopy->cells[idToAdd]);
		}
	}
	return mapCopy;
}

/*
	Libere proprement la mémoire occupé par la map
*/
void freeMap(SMap *map){
	if(map != NULL){
		for(int i=0 ; i  < map->nbCells ; i++){
			free(map->cells[i].neighbors);
		}
		free(map->cells);
		free(map);
	}
}

// Affichage des régles en cas de problème de paramètre au lancement
void rappelSyntaxe(char* affichage){
	printf("%s",affichage);
	printf("Parametre 1 : Nombre de parties souhaitees (max 9)\n");
	printf("Parametre 2 : Nombre de joueurs souhaites (max 8)\n");
	printf("Parametre 3 et + : Fichiers d'IA a utiliser (max 2)\n");
	printf("Exemple : ./AlphaDice 2 6\n");
}
