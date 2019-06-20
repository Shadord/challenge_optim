#include "stdio.h"
#include "string.h"

void initContener(int nb_conteneurs, int largeur, int hauteur) {
    FILE* fp = NULL;
    fp = fopen("instances/1_global.csv", "r");


    if (fp == NULL) {
        printf("\nImpossible de charger l'instance");
        return 0;
    }
    do {
        fscanf(fp, "%d , %d , %d", nb_conteneurs, largeur, hauteur);
    } 

    fclose(fp);
    return 1;
  }



