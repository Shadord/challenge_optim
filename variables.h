
typedef struct {
	int id_container;
  int posX;
  int posY;
  char name[];
} Container;

typedef struct {
  int nb_container;
  int height;
  int size;
	Container **baie;
} Port;
