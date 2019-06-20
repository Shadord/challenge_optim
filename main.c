#include "stdio.h"
#include "string.h"

void initContener() {
    FILE* fp = NULL;
    int nbmots = 0, numMotChoisi = 0;
    int caractereLu = 0;
    fp = fopen("instances/1_global.csv", "r");


    if (fp == NULL) {
        printf("\nImpossible de charger l'instance");
        return 0;
    }
    do {
        fscanf(fp, "%d , %d , %d", )
    } while(caractereLu != EOF);


    fclose(fp);
    return 1;
  }
