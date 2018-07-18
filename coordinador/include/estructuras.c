#include "estructuras.h"

int void_ptr_to_int(void * p) {
    return p ? *((int *) p) : 0;
}

int buscar_clave(t_dictionary *dict, char *clave) {
    return void_ptr_to_int(dictionary_get(dict, clave));
}

int obtener_instancia(t_list *list, char *clave) {
    char letra = *clave;

    // Cuando no tenes aplicacion parcial y tenes que hacer esto 😭
    bool empieza_con(info_instancia *ii) {
        char letra_inicial = (ii->empieza_con)[LETRA_INICIAL];
        char letra_final = (ii->empieza_con)[LETRA_FINAL];

        return letra >= letra_inicial && letra <= letra_final;
    }

    return void_ptr_to_int(list_find(list, (void *) empieza_con));  
}