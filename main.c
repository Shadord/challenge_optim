#include "stdio.h"
#include "string.h"
#include <variable.h>

void initContainer(Port *P, Data *D) {

      FILE* fp = NULL;
      fp = fopen("instances/1_global.csv", "r");
      fseek(fp, 10, SEET_SET);

      if (fp == NULL) {
        printf("\nImpossible de charger l'instance");
        return 0;
      }

      fscanf(fp, "%d , %d , %d",P->nb_container, P->width, P->height);

      fclose(fp);
      return 1;
}

P->baie = (Container***) malloc(P->width * sizeof(Container**));
D->liste_containers = (Container**) malloc(P->nb_container * sizeof(Container*));
for(int i = 0; i < P->width; i++) {
  P->baie[i] = (Container**) malloc(P->height * sizeof(Container*));
}
P->heights = (int*) malloc(P->width * sizeof(int));
// Donc si on résume P[x][y] est un Container et pas une adresse de container

void addContainer(Port *P, Container *C) {
      int i = 0;
      while(i < P->size) {

      }
}

void draw(Port *P) {
    for(int i = 0; i < P->height; i++) { // Les hauteurs
      for(int j = 0; i < P->width; i++) { // Les largeurs
        if(P->baie[j][i]->id_container < 10){
          printf(" %s ", P->baie[j][i]->name);
        }else{
          printf(" %s", P->baie[j][i]->name);
        }
      }
      printf("\n");
    }
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
