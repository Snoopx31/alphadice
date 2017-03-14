#include <stdio.h>
#include <stdlib.h>
#include "arbre.h"


struct Noeud{
	Data Value;
	Noeud *Fils;
	int nbFils;
};

struct SArbre{
	Noeud *Head;
};

SArbre* CreateArbre(int taille){
	SArbre *arbre = malloc(sizeof(Noeud));
	Noeud *newNoeud = malloc(sizeof(Noeud));

	Noeud listeFils[taille];

	newNoeud->nbFils = taille;
	newNoeud->Fils = listeFils;
	newNoeud->Value = 12;

	arbre->Head = newNoeud;
	return arbre;
}

Noeud* headOfArbre(SArbre *arbre){
	return arbre->Head;
}

Noeud* getFils(Noeud *noeud, int numero){
	return (noeud->Fils) + numero;
}

void DeleteArbre(SArbre *arbre){
	free(arbre);
}


void AddElement(Noeud *noeud, Data *elem, int nbElement){
	Noeud *newNoeuds = malloc(sizeof(Noeud)*nbElement);
	noeud->Fils = newNoeuds;
	noeud->nbFils = nbElement;

	for(int i = 0; i < nbElement; i++){
		newNoeuds->Value = *elem;
		newNoeuds++;
		elem++;
	}
}

int sub(Noeud *noeud, int niv, int tab[],int taille){
	//printf("%d\n",niv);
	if (niv == 0){
		return 0;
	}
	niv--;
	Noeud *fils;
	for(int i = 0; i < taille; i++){
		fils = getFils(noeud,i);
		AddElement(fils,tab,taille);
		sub(fils,niv,tab,taille);
	}
	return 1;
}

void creation(SArbre *arbre, int niveau){
	int tab[10] = {45,2};
	Noeud *head = headOfArbre(arbre);
	AddElement(head,tab,2);
	sub(head,--niveau,tab,2);
}
/*
Noeud* AddElementEnd(SArbre *arbre,Data elem){
	Noeud *newNoeud = malloc(sizeof(elem));

	newNoeud->Value = elem;
	newNoeud->suivant = NULL;

	if(arbre == NULL){
		arbre->Head = newNoeud;
		return newNoeud;
	}
	Noeud *temp = arbre->Head;
	while(temp->suivant != NULL){
		temp = temp->suivant;
	}
	temp->suivant = newNoeud;
	return newNoeud;
}

Noeud* AddElementAfter(SArbre *arbre,Noeud *noeud,Data elem){
	Noeud *newNoeud = malloc(sizeof(elem));

	newNoeud->Value = elem;
	if(arbre->Head == NULL){
		arbre->Head = newNoeud;
		newNoeud->suivant = NULL;
		newNoeud->precedent = NULL;
		return newNoeud;
	}

	newNoeud->precedent = noeud;
	newNoeud->suivant = noeud->suivant;
	noeud->suivant = newNoeud;

	return newNoeud;
}

void DeleteNoeud(SArbre *arbre,Noeud *noeud){
	if(arbre->Head == NULL){
		return;
	}
	Noeud *temp = arbre->Head;
	Noeud *last;
	while(temp != noeud && temp->suivant != NULL){
		last = temp;
		temp = temp->suivant;
	}
	temp->suivant->precedent = last;
	last->suivant = temp->suivant;
	free(noeud);
}

Noeud* GetFirstElement(SArbre *arbre){
	if(arbre == NULL){
		return NULL;
	}
	return arbre->Head;
}

Noeud* GetLastElement(SArbre *arbre){
	if(arbre == NULL){
		return NULL;
	}
	return arbre->Head->precedent;
}

Noeud* GetPrevElement(Noeud *noeud){
	return noeud->precedent;
}

Noeud* GetNextElement(Noeud *noeud){
	return noeud->suivant;
}
*/
Data getData(Noeud *noeud){
	if(noeud != NULL){
		return noeud->Value;
	}
	return NULL;
}