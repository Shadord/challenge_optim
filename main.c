#include "stdio.h"
#include "string.h"

void initContainerr() {
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

void placerContainer(Port *P) {
  int caractereLu = 0;
  int ligne = 0;
  FILE *fp = NULL;
  fp = fopen("instances/1_position.csv", "r");

  if (fp == NULL) {
      printf("\nImpossible de charger l'instance");
      return 0;
  }

  while (caractereLu != EOF){
    if (ligne>0)
      {
        Container *C = (Container *)malloc(sizeof(Container));
        fseek(fp, 21, SEEK_SET);
        fscanf(fp, "%s, %d, %d", C->name, C->posX, C->posY);
        C->id_container = ligne;
        P->baie[PosX-1][P->height-PosY]=C;
      }
    ligne++;
  }

}
