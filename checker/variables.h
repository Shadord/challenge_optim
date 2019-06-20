
typedef struct {
	int id_container;
  int posX;
  int posY;
  char name[20];
  int placed;
} Container;

typedef struct {
  int nb_container; // Nombre de container dans le port
  int maxHeight; // Hauteur max (taille max d'une colonne)
  int maxWidth; // Taille des lignes
  int *heights; // Contient la taille actuelle de chaque colonne
	Container ***baie; // Matrice d'adresse des containers
} Port;

typedef struct {
  Container **liste_containers;
} Data;
