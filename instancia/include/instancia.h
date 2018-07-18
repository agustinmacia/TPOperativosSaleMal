#ifndef INSTANCIA_H_
#define INSTANCIA_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "algoritmos.h"
#include "../../utils/sockets.h"

typedef struct cfg {
	char* ip_coordinador;
	char* puerto_coordinador;
	Algoritmo_reemplazo algoritmo;
	char* punto_de_montaje;
	int intervalo_dump;
} Instancia_config;

t_log * logger;

Instancia_config iniciar_config(char* path);
void configure_logger();

void manejador(int socket);

#endif /*INSTANCIA_H_ */
