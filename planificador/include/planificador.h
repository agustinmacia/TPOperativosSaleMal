#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

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
#include "../../utils/sockets.h"
#include "consola.h"
//#include "../utils/readerFiles.h"
#include "estructuras.h"


typedef struct cfg {
	char * ip_planificador;
	char * puerto_planificador;
	int estimacion_inicial;
} Planificador_config;

//Defino variable logger
t_log * logger;


//Declaro prototipo de funciones
Planificador_config iniciar_config(char* path);
void* manejar_sockets(void *vargp);
void manejador(int socket);
void configure_logger();

#endif /*PLANIFICADOR_H_ */
