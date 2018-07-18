#include "planificador.h"

/**
 * @DESC: Va a buscar un archivo de configuracion en el *path* y devuelve una
 * estructura que contiene la informacion parseada. Ante error devuelve una
 * configuracion default.
 *
 * @PARAMS:
 *		path - el archivo al que hay que ir a buscar la configuracion.
*/

Planificador_config iniciar_config(char* path) {

	t_config *cfg = config_create(path);

	Planificador_config config;

	if (!cfg) {
		config.ip_planificador = "127.0.0.1";
		config.puerto_planificador = "8081";
        config.estimacion_inicial = 1;

	} else {

		config.ip_planificador = config_get_string_value(cfg, "ip_planificador");
		config.puerto_planificador = config_get_string_value(cfg, "puerto_planificador");
        config.estimacion_inicial = config_get_int_value(cfg, "estimacion_inicial");
	}

	free(cfg);

	return config;
}

void* manejar_sockets(void *vargp) {
  Planificador_config CONFIG = iniciar_config("planificador.cfg");

  configure_logger();

	int socket = crear_socket();
	if (escuchar_en(socket, CONFIG.puerto_planificador) == 0) {
		obtener_informacion(socket, manejador);
	}
}

void manejador(int socket) {
    while(true) {

        mensaje_t* mensaje = recv_mensaje(socket);

        if(mensaje->header == H_SOY_ESI){
            log_info(logger, "SOCKET ESI %d ", socket);
            log_info(logger, "------------------------------------------------");

            // Hay que agregarlo a la lista de listos
            break;

        } else {
            log_error(logger, "CONEXION ERRONEA - SOCKET %d", socket);
            close(socket);
        }
    }

    // SUPONIENDO QUE AGARRO UN ESI DE LA LISTA:
    enviar_notificacion(socket, H_COMENZAR);

    while (recv_notificacion(socket) == H_SIGUIENTE) {
        log_info(logger, "Leyendo ESI");
        enviar_notificacion(socket, H_LEER);
    }

    if(recv_notificacion(socket) == H_TERMINADO){
        log_info(logger, "ESI TERMINADO %d ", socket);
        log_info(logger, "------------------------------------------------");
        log_info(logger, "------------------------------------------------");
    
    }
}


void configure_logger() {
	logger = log_create("planificador.log", "planificador", true, LOG_LEVEL_INFO);
}
