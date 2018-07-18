#include "estructuras.h"

info_esi *crear_esi(int socket, int estimacion_inicial) {
    info_esi *esi = malloc(sizeof(info_esi));
    esi->socket = socket;
    esi->claves = NULL;
    esi->estimacion = estimacion_inicial;
    return esi;
}

void destruir_esi(info_esi **esi) {
    // Si, este if es una mierda, es para evitar segfaults ante NULLs
    if(esi && *esi) {
        if((*esi)->claves) {
            list_destroy_and_destroy_elements((*esi)->claves, (void*) destruir_clave);
        }
        free(*esi);
        *esi = NULL;
    }
}

void destruir_clave(char* clave) {
    free(clave);
}

int agregar_esi(t_list *lista, info_esi *esi) {
    return list_add(lista, esi);
}

info_esi* encontrar_esi(t_list *lista, int socket) {
    bool _tiene_id(info_esi *p) {
        return p->socket == socket;
    }
    return (info_esi*) list_find(lista, (void*) _tiene_id);
}

info_esi *remover_esi(t_list *lista, int socket) {
    // Aguante repetir logica, si alguno sabe como devolver un puntero a 
    // funcion de una funcion esta invitado a arreglar esta mierda
    bool _tiene_id(info_esi *p) {
        return p->socket == socket;
    }
    void * data = list_remove_by_condition(lista, (void*) _tiene_id);
    return (info_esi *) data;
}

info_esi *mover_esi(t_list *origen, t_list*destino, int socket) {
    info_esi *aux = remover_esi(origen, socket);
    list_add(destino, aux);
    return aux;
}

void *remover_esi_destruyendolo(t_list *lista, int socket) {
    info_esi *esi = remover_esi(lista, socket);
    destruir_esi(&esi);
}

bool esi_equals(info_esi* uno, info_esi* otro) {
    if (!uno) {
        if(!otro) return true;
        return false;
    }
    return uno->socket == otro->socket;
}