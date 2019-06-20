#include "stdio.h"
#include "string.h"
#include "variables.h"
#include "stdlib.h"

#define INSTANCE 20

int instance = 1;

int moves = 0;

FILE *fpe = NULL;

int extFichier(int back, int new){
  fprintf(fpe, "%d , %d\n", back+1, new+1);
}

int isReady(Port *P, Container *C);
void addContainer(Port *P, Container *C_a_add);
void addContainercolumn(Port *P, Container *C_a_add);

int initContainer(Port *P) {
      char filename[40];
      sprintf(filename, "instances/%d_global.csv", instance);
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
    for(int j = 0; j < P->maxWidth; j++) {
      printf("  %d  ", P->heights[j]);
    }
    printf("Nombre de déplacements : %d\n", moves);
  }

int placerContainer(Port *P, Data *D) {
  char filename[40];
  sprintf(filename, "instances/%d_position.csv", instance);
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
        C->posX = 0;
        C->posY = 0;
      }else{
        C->placed = 1;
        P->baie[C->posX][C->posY] = C;
        P->heights[C->posX]++;
      }
      D->liste_containers[i] = C;
  }
}

int min(Port *P, int indice){
  int minimum_colonne = 1000;
  for(int i = 0; i < P->maxHeight; i++){
    Container *C_colonne_min = P->baie[indice][i];
    if(C_colonne_min != NULL){
      if(minimum_colonne > C_colonne_min->id_container){
        minimum_colonne = C_colonne_min->id_container;

      }

    }
  }

  return minimum_colonne;

}

int isLowerContain(Port *P, Container *C_a_test) {
  int id = C_a_test->id_container;
  int minimum_colonne = -1;
  for(int i = 0; i < P->maxWidth; i++){
    minimum_colonne = min(P, i);
    if(id < minimum_colonne && P->heights[i] < P->maxHeight){
      return i;
    }

  }
  return -1;


}


int moveContainer(Port *P, Container *C) {
  int moved = 0;
  int back;
  while(1){
    if(isReady(P, C)){
      P->baie[C->posX][C->posY]=NULL; // enlever le conteneur de l'ancienne nouv_pile
      P->heights[C->posX]--;
      addContainercolumn(P,C);
      moves++;
      break;
/*
      for(int i = 0; i < P->maxWidth; i++) {
        if(P->heights[i] < P->maxHeight && i != C->posX ){ //on déplace le container dans la première colonne libre différente de la précédente
          C->placed = 1;
          back = C->posX;
          C->posX = i;
          extFichier(back, C->posX);
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
      }*/
    }else{
      moveContainer(P, P->baie[C->posX][C->posY-1]);
    }
  }

}

int checkContainer(Port *P){ // regarde si on peux faire un move de contener
  printf("Check container\n");
  int back;
  int result = 0;
  for(int i = 0; i < P->maxWidth; i++){ // Pour chaque colonne
    Container *C_haut = P->baie[i][P->maxHeight-P->heights[i]];
    if(P->heights[i] != 0){
      if(C_haut->id_container != min(P, i) && P->heights[i] < P->maxHeight){
        int dpl = isLowerContain(P, C_haut); // Si le container le plus haut et bougeable dpl != 0
        if(dpl != -1 && dpl != C_haut->posX) { //Le container est bougeable dans la colonne dpl
          P->baie[C_haut->posX][C_haut->posY]=NULL; // enlever le conteneur de l'ancienne nouv_pile
          P->heights[C_haut->posX]--;
          back = C_haut->posX;
          C_haut->posX = dpl;
          extFichier(back, C_haut->posX);
          C_haut->posY = P->maxHeight-P->heights[dpl]-1;
          printf("Optimisation du container %s à [%d, %d]\n", C_haut->name, C_haut->posX, C_haut->posY);
          P->baie[C_haut->posX][C_haut->posY] = C_haut;
          moves++;
          P->heights[C_haut->posX]++;
          result = 1;
          break;
        }
      }
    }
  }
  return result;
}

void addContainercolumn(Port *P, Container *C_a_add){
  int back;
  for (int i=0; i<P->maxWidth;i++){
      int dpl = isLowerContain(P, C_a_add);
      if(dpl != -1 && P->heights[dpl] < P->maxHeight) {
        back = C_a_add->posX;
        C_a_add->posX = dpl;
        extFichier(back, C_a_add->posX);
        C_a_add->posY = P->maxHeight-P->heights[dpl]-1;
        printf("Ajout du container %s à [%d, %d]\n", C_a_add->name, C_a_add->posX, C_a_add->posY);
        P->baie[C_a_add->posX][C_a_add->posY] = C_a_add;
        P->heights[C_a_add->posX]++;
        return;
      }
    }
    for(int i = 0; i < P->maxWidth; i++){
      if(P->heights[i] < P->maxHeight && i != C_a_add->posX){
        back = C_a_add->posX;
        C_a_add->posX = i;
        extFichier(back, C_a_add->posX);
        C_a_add->placed = 1;
        C_a_add->posY = P->maxHeight-P->heights[i]-1;
        printf("Ajout du container %s às [%d, %d]\n", C_a_add->name, C_a_add->posX, C_a_add->posY);
        P->baie[C_a_add->posX][C_a_add->posY] = C_a_add;
        P->heights[C_a_add->posX]++;
        break;
      }
  }
}

void addContainer(Port *P, Container *C_a_add){
  int back;
  for(int i = 0; i < P->maxWidth; i++){
    int dpl = isLowerContain(P, C_a_add);
    if(dpl != -1 && P->heights[dpl] < P->maxHeight) {
      back = C_a_add->posX;
      C_a_add->posX = dpl;
      extFichier(back-1, C_a_add->posX);
      C_a_add->posY = P->maxHeight-P->heights[dpl]-1;
      printf("Ajout du container %s à [%d, %d]\n", C_a_add->name, C_a_add->posX, C_a_add->posY);
      P->baie[C_a_add->posX][C_a_add->posY] = C_a_add;
      P->heights[C_a_add->posX]++;
      return;
    }
  }
  for(int i = 0; i < P->maxWidth; i++){
    if(P->heights[i] < P->maxHeight){
      back = C_a_add->posX;
      C_a_add->posX = i;
      extFichier(back-1, C_a_add->posX);
      C_a_add->placed = 1;
      C_a_add->posY = P->maxHeight-P->heights[i]-1;
      printf("Ajout du container %s às [%d, %d]\n", C_a_add->name, C_a_add->posX, C_a_add->posY);
      P->baie[C_a_add->posX][C_a_add->posY] = C_a_add;
      P->heights[C_a_add->posX]++;
      break;
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
    extFichier(C->posX, -1);
    return 1;
  }else{
    return 0;
  }
}

int operations(Port *P, Data *D) {
  char filename[40];
  sprintf(filename, "instances/%d_operations.csv", instance);
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
        if(id[3] != NULL){
          ids = ids*10 + ((int)id[3] - 48);
          if(id[4] != NULL){
            ids = ids*10 + ((int)id[4] - 48);
          }
        }
        if(instance > 10){
          while(checkContainer(P));
        }else{
          checkContainer(P);
        }
        if(isReady(P, D->liste_containers[ids-1]) == 0){
          moveContainer(P, P->baie[D->liste_containers[ids-1]->posX][D->liste_containers[ids-1]->posY-1]);
        }
        removeContainer(P, D->liste_containers[ids-1]);
      }

      if(c == 'A'){
        ids = (int)id[2] - 48;
        if(id[3] != NULL){
          ids = ids*10 + ((int)id[3] - 48);
          if(id[4] != NULL){
            ids = ids*10 + ((int)id[4] - 48);
          }
        }
        if(instance > 10){
          while(checkContainer(P));
        }else{
          checkContainer(P);
        }

        addContainer(P, D->liste_containers[ids-1]);
      }
      i++;
      draw(P);
  }
}





int main(int argc, char const *argv[]) {
  int resultats[20];
  char filename[30];

  for(int i = 1; i < 21; i++){
    sprintf(filename, "%d_solution.csv", instance);
    fpe = fopen(filename, "wr");
    fprintf(fpe, "FROM , TO\n");
    Port P;
    Data D;
    initContainer(&P);
    placerContainer(&P, &D);
    draw(&P);
    operations(&P, &D);
    printf("Instance %d = %d déplacements\n", instance, moves);
    resultats[i] = moves;
    moves = 0;
    instance++;
  }
  for(int i = 1; i < 21; i++){
    printf("Instance %d = %d déplacements\n", i, resultats[i]);
  }
  // ./checker_unix 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20

  /*
  Port P;
  Data D;
  initContainer(&P);
  placerContainer(&P, &D);
  draw(&P);
  operations(&P, &D);
  printf("Déplacements : %d\n", moves);*/
  return 0;
}
