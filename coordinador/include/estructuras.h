#ifndef _H_ESTRUCTURAS_C
#define _H_ESTRUCTURAS_C

#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <stdbool.h>

#define LETRA_INICIAL 0
#define LETRA_FINAL   1

typedef struct {
	int socket;
    char empieza_con[2];
} info_instancia;

// La key es la clave, el value es el socket de la instancia donde esta
t_dictionary indice_de_claves;

int void_ptr_to_int(void * p);
int buscar_clave(t_dictionary *dict, char *clave);
int obtener_instancia(t_list *list, char *clave);

#endif
