#include "instancia.h"

/**
 * @DESC: Va a buscar un archivo de configuracion en el *path* y devuelve una
 * estructura que contiene la informacion parseada. Ante error devuelve una
 * configuracion default.
 *
 * @PARAMS:
 * 		path - el archivo al que hay que ir a buscar la configuracion.
 */
Instancia_config iniciar_config(char* path) {

	t_config *cfg = config_create(path);

	Instancia_config config;

	if (!cfg) {

		config.ip_coordinador = "127.0.0.1";
		config.puerto_coordinador = "8080";

		config.algoritmo=BSU;
		config.punto_de_montaje="/home/utnso/instanciaX";
		config.intervalo_dump=10;

	} else {

		config.ip_coordinador = config_get_string_value(cfg, "ip_coordinardor");
		config.puerto_coordinador = config_get_string_value(cfg, "puerto_coordinador");
		config.algoritmo=BSU;
		config.punto_de_montaje="/home/utnso/instanciaX";
		config.intervalo_dump=10;
	}

	free(cfg);

	return config;
}

void manejador(int socket) {
	while (true) {
		mensaje_t* mensaje = recv_mensaje(socket);

		if(mensaje->header == H_GET){
			log_info(logger, "EJECUTO GET...", socket);
			log_info(logger, "* Clave: %s", mensaje->clave);
			log_info(logger, "------------------------------------------------");
			enviar_notificacion(socket, H_EXITO);
		} else if(mensaje->header == H_STORE){
			log_info(logger, "EJECUTO STORE...", socket);
			log_info(logger, "* Clave: %s", mensaje->clave);
			log_info(logger, "------------------------------------------------");
			enviar_notificacion(socket, H_EXITO);
		} else if(mensaje->header == H_SET){
			log_info(logger, "EJECUTO SET...", socket);
			log_info(logger, "* Clave: %s", mensaje->clave);
			log_info(logger, "* Valor: %s", (mensaje->payload_set)->valor);
			log_info(logger, "------------------------------------------------");
			enviar_notificacion(socket, H_EXITO);
		} else {
			log_error(logger, "CONEXION ERRONEA - SOCKET %d", socket);
			close(socket);
		}
	}
}

void configure_logger() {
	logger = log_create("instancia.log", "instancia", true, LOG_LEVEL_INFO);
}
