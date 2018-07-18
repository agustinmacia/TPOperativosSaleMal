#ifndef ESI_H_
#define ESI_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <parsi/parser.h>
#include "../../utils/protocolo.h"
#include "../../utils/sockets.h"
//#include "../../utils/readerFiles.h"

typedef struct cfg {
	char * ip_coordinador;
	char * puerto_coordinador;
	char * ip_planificador;
	char * puerto_planificador;
} Esi_config;

//Defino variable logger
t_log * logger;

//Declaro prototipo de funciones
Esi_config iniciar_config(char* path);
void configure_logger();
int leer_archivo(char *archivo, int socket_planificador, int socket_coordinador);

#endif /*ESI_H_ */
