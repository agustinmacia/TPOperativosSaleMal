#ifndef COORDINADOR_H_
#define COORDINADOR_H_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "algoritmos.h"
#include "../../utils/sockets.h"
//#include "../utils/readerFiles.h"

typedef struct cfg {
	char *puerto;
	Algoritmo_distribucion algoritmo;
	int entradas;
	int tamanio;
	int retardo;
} Coordinador_config;

t_log* logger;

//Declaro prototipo de funciones
const Coordinador_config iniciar_config(char* path);
void manejador(int socket);
t_log* configure_logger();
void ejecutar_en_instancia(mensaje_t* mensaje);
int seleccionar_socket();

t_list* instancias;

#endif /* COORDINADOR_H_ */
