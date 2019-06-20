#include "stdio.h"
#include "string.h"
#include "variables.h"
#include "stdlib.h"

#define INSTANCE 20

int initContainer(Port *P) {
      char filename[40];
      sprintf(filename, "instances/%d_global.csv", INSTANCE);
      FILE* fp = NULL;
      fp = fopen(filename, "r");


      if (fp == NULL) {
        printf("\nImpossible de charger l'instance");
        return 0;
      }
      fseek(fp, 10, SEEK_SET);

      fscanf(fp, "%d , %d , %d",&P->nb_container, &P->maxWidth, &P->maxHeight);

      fclose(fp);
      return 1;
}

void draw(Port *P) {
    printf("|");
    for(int i = 0; i < P->maxHeight; i++) { // Les hauteurs
      for(int j = 0; j < P->maxWidth; j++) { // Les largeurs
        if(P->baie[j][i] != NULL) {
          if(P->baie[j][i]->id_container < 10){
            printf(" %s ", P->baie[j][i]->name);
          }else{
            printf(" %s", P->baie[j][i]->name);
          }
        }else{
          printf("     ");
        }
      }
      printf("|\n|");
    }
  }

int placerContainer(Port *P, Data *D) {
  char filename[40];
  sprintf(filename, "instances/%d_position.csv", INSTANCE);
  FILE *fp = NULL;
  fp = fopen(filename, "r");


  if (fp == NULL) {
      printf("\nImpossible de charger l'instance");
      return 0;
  }

  P->baie = (Container***) malloc(P->maxWidth * sizeof(Container**));
  D->liste_containers = (Container**) malloc(P->nb_container * sizeof(Container*));
  for(int i = 0; i < P->maxWidth; i++) {
    P->baie[i] = (Container**) malloc(P->maxHeight * sizeof(Container*));
  }
  P->heights = (int*) malloc(P->maxWidth * sizeof(int));
  for(int i = 0; i < P->maxWidth; i++){
    P->heights[i] = 0;
  }
  // Donc si on résume P[x][y] est un Container et pas une adresse de container

  int i = 1;

  fseek(fp, 21, SEEK_SET);

  for(int i = 0; i < P->nb_container; i++){
      Container *C = (Container *)malloc(sizeof(Container));
      fscanf(fp, "%s , %d , %d", C->name, &C->posX, &C->posY);
      printf("%s -- %d -- %d\n", C->name, C->posX, C->posY);
      C->id_container = i+1;
      if(C->posX == 0 && C->posY == 0){
        C->placed = 0;
      }else{
        C->placed = 1;
        P->baie[C->posX-1][P->maxHeight-C->posY] = C;
        P->heights[C->posY]++;
      }
      D->liste_containers[i] = C;
  }
}

int moveContainer(Port P, Container C) {
  int former_PosX = posX-1; // position x dans le matrice
  int former_PosY = P->maxHeight-posY; // position y dans la matrice
  P->baie[former_PosX-1][P->maxHeight-former_PosY]=NULL; // enlever le conteneur de l'ancienne nouv_pile
  nouv_PosX = 0;
  while (heights[nouv_PosX]<4 ||nouv_PosX!=formerPosX){

  }

}

int addContainer(Port *P, Container *C) {
  if(C->placed == 1){
    printf("The container %s is already placed\n", C->name);
  }
  for(int i = 0; i < P->maxWidth; i++) {
    if(P->heights[i] < P->maxHeight){ // On peut ajouter le container dans cette colonne
      P->baie[i][P->maxHeight-P->heights[i]-1] = C;
      C->placed = 1;
      printf("The container %s has been placed at [%d, %d]\n", C->name, i, P->maxHeight-P->heights[i]-1);
    }
  }
}




int main(int argc, char const *argv[]) {
  Port P;
  Data D;
  initContainer(&P);
  placerContainer(&P, &D);
  draw(&P);
  return 0;
}
