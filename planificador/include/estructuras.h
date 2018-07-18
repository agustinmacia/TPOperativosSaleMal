#ifndef _H_ESTRUCTURAS
#define _H_ESTRUCTURAS

#include <commons/collections/list.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int socket;
    int estimacion;
    t_list *claves;
} info_esi;

info_esi *en_ejecucion;
t_list *listos;
t_list *finalizados;
t_list *bloqueados;

/**
 * Constructor de info_esi (Es WIP, va a terminar tomando mas parametros)
 */
info_esi *crear_esi(int socket, int estimacion_inicial);

/**
 * Destructor de info_esi.
 */
void destruir_esi(info_esi **esi);

/**
 * Esta medio al pedo esto, pero es la manera que tenemos de destruir la
 * lista de claves de info_esi
 */
void destruir_clave(char* clave);

/**
 * Agrega un esi a una cola específica (podriamos "sobrecargarlo" para
 * agregarlo a las colas ya conocidas, tipo agregar_esi_bloqueado?).
 *
 * Devuelve el indice del elemento recien creado
 */
int agregar_esi(t_list *lista, info_esi *esi);

info_esi* encontrar_esi(t_list *lista, int socket);
int encontrar_esi_por_indice(t_list *lista, int indice);

/**
 * Remueve un info_esi de una cola, buscandolo por id.
 */
info_esi *remover_esi(t_list *lista, int socket);

/**
 * Muevo un esi de una cola a otra (listos -> bloqueados, por ejemplo).
 * Lo copado de esto es que remueve de una y mueve a la otra, lo que nos
 * saca un problema de normalizacion (un mismo puntero esta en dos listas).
 */
info_esi *mover_esi(t_list *origen, t_list*destino, int socket);

/**
 * Hace remover_esi, pero tambien destruye al info_esi
 */
void *remover_esi_destruyendolo(t_list *lista, int socket);

bool esi_equals(info_esi* uno, info_esi* otro);
#endif