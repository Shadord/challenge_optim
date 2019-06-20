#include "stdio.h"
#include "string.h"
#include "variables.h"
#include "stdlib.h"

#define INSTANCE 1

int moves = 0;

int isReady(Port *P, Container *C);

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
      int x, y;
      Container *C = (Container *)malloc(sizeof(Container));
      fscanf(fp, "%s , %d , %d", C->name, &x, &y);
      C->posX = x-1;
      C->posY = P->maxHeight - y;
      printf("%s -- %d -- %d\n", C->name, C->posX, C->posY);
      C->id_container = i+1;

      if(C->posX == -1){
        C->placed = 0;
      }else{
        C->placed = 1;
        P->baie[C->posX][C->posY] = C;
        P->heights[C->posX]++;
      }
      D->liste_containers[i] = C;
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

int moveContainer(Port *P, Container *C) {
  int moved = 0;
  while(1){
    if(isReady(P, C)){
      P->baie[C->posX][C->posY]=NULL; // enlever le conteneur de l'ancienne nouv_pile
      P->heights[C->posX]--;

      for(int i = 0; i < P->maxWidth; i++) {
        if(P->heights[i] < P->maxHeight && i != C->posX ){ //on déplace le container dans la première colonne libre différente de la précédente
          C->placed = 1;
          C->posX = i;
          C->posY = P->maxHeight-P->heights[i]-1;
          P->heights[i]++;
          P->baie[C->posX][C->posY] = C;
          printf("The container %s has been placed at [%d, %d]\n", C->name, C->posX, C->posY);
          moved = 1;
          moves++;
          break;
        }
      }
      if(moved == 1){
        break;
      }
    }else{
      moveContainer(P, P->baie[C->posX][C->posY-1]);
    }
  }

}



int isReady(Port *P, Container *C) {
  if(C->posY > 0){
    if(P->baie[C->posX][C->posY-1] == NULL) {
      printf("The container %s is ready to move\n", C->name);
      return 1;
    }else{
      printf("The container %s is not ready to move\n", C->name);
      return 0;
    }
  }else{
    printf("The container %s is ready to move\n", C->name);
    return 1;
  }

}

int removeContainer(Port *P, Container *C){
  if(isReady(P, C)){
    P->baie[C->posX][C->posY]=NULL; // enlever le conteneur de l'ancienne nouv_pile
    P->heights[C->posX]--;
    C->placed = 0;
    return 1;
  }else{
    return 0;
  }
}

int operations(Port *P, Data *D) {
  char filename[40];
  sprintf(filename, "instances/%d_operations.csv", INSTANCE);
  FILE *fp = NULL;
  fp = fopen(filename, "r");


  if (fp == NULL) {
      printf("\nImpossible de charger l'instance");
      return 0;
  }
  // Donc si on résume P[x][y] est un Container et pas une adresse de container

  int i = 1;

  fseek(fp, 11, SEEK_SET);
  char id[20];
  int ids;
  char c;
  while(fscanf(fp, "%s , %c", id, &c) != EOF ){
      printf("%s -- %c\n", id, c);
      if(c == 'R'){
        ids = (int)id[2] - 48;
        printf("%d\n", ids);
        if(id[3] != NULL){
          ids = ids*10 + ((int)id[3] - 48);
        }
        printf("%d\n", ids);
        if(isReady(P, D->liste_containers[ids-1]) == 0){
          moveContainer(P, P->baie[D->liste_containers[ids-1]->posX][D->liste_containers[ids-1]->posY-1]);

        }
        removeContainer(P, D->liste_containers[ids-1]);
      }
      i++;
      draw(P);
  }
}



int main(int argc, char const *argv[]) {
  Port P;
  Data D;
  initContainer(&P);
  placerContainer(&P, &D);
  operations(&P, &D);
  printf("Déplacements : %d\n", moves);
  return 0;
}
