#include "coordinador.h"
#include <commons/collections/list.h>

/**
 * @DESC: Va a buscar un archivo de configuracion en el *path* y devuelve una
 * estructura que contiene la informacion parseada. Ante error devuelve una
 * configuracion default.
 *
 * @PARAMS:
 *              path - el archivo al que hay que ir a buscar la configuracion.
 */
const Coordinador_config iniciar_config(char* path) {

    // Valores default que zafan.

    Coordinador_config CONFIG = { .puerto = "8080",
                                                             .algoritmo = EL,
                                                             .entradas = 20,
                                                             .tamanio = 100,
                                                             .retardo = 300 };

    t_config *cfg = config_create(path);

    // El archivo es valido
    if (cfg) {

            CONFIG.puerto = config_get_string_value(cfg, "puerto");
            CONFIG.algoritmo = algoritmo_from_string(config_get_string_value(cfg, "algoritmo"));
            CONFIG.entradas = config_get_int_value(cfg, "entradas");
            CONFIG.tamanio = config_get_int_value(cfg, "tamanio");
            CONFIG.retardo = config_get_int_value(cfg, "retardo");
    }

    free(cfg);
    return CONFIG;
}

void manejador(int socket) {

    while (true) {
        mensaje_t* mensaje = recv_mensaje(socket);

        if(mensaje->header == H_SOY_ESI){
            log_info(logger, "SOCKET ESI %d ", socket);
            log_info(logger, "------------------------------------------------");
        } else if(mensaje->header == H_GET){
            log_info(logger, "HAY UN GET %d - HANDSHAKE REALIZADO", socket);
            log_info(logger, "* Tamanio clave: %d", mensaje->tamanio_clave);
            log_info(logger, "* Clave: %s", mensaje->clave);
            enviar_notificacion(socket, H_EXITO);
            log_info(logger, "------------------------------------------------");
						enviar_a_instancia(mensaje);
						enviar_notificacion(socket, H_EXITO);
        } else if(mensaje->header == H_STORE){
            log_info(logger, "HAY UN STORE %d - HANDSHAKE REALIZADO", socket);
            log_info(logger, "* Tamanio clave: %d", mensaje->tamanio_clave);
            log_info(logger, "* Clave: %s", mensaje->clave);
            log_info(logger, "------------------------------------------------");
						enviar_a_instancia(mensaje);
						enviar_notificacion(socket, H_EXITO);
        } else if(mensaje->header == H_SET){
            log_info(logger, "HAY UN SET %d - HANDSHAKE REALIZADO", socket);
            log_info(logger, "* Tamanio clave: %d", mensaje->tamanio_clave);
            log_info(logger, "* Clave: %s", mensaje->clave);
            log_info(logger, "* Payload tamanio: %d", (mensaje->payload_set)->tamanio_valor);
            log_info(logger, "* Payload valor: %s", (mensaje->payload_set)->valor);
            log_info(logger, "------------------------------------------------");
						enviar_a_instancia(mensaje);
						enviar_notificacion(socket, H_EXITO);
        } else if (mensaje->header == H_TERMINADO){
            log_info(logger, "ESI TERMINADO %d ", socket);
            log_info(logger, "------------------------------------------------");
            log_info(logger, "------------------------------------------------");
            break;
        } else if(mensaje->header == H_SOY_INSTANCIA){
            log_info(logger, "SOCKET INSTANCIA %d ", socket);
            log_info(logger, "------------------------------------------------");

						list_add(instancias, socket);
            break;
        } else {
            log_error(logger, "CONEXION ERRONEA - SOCKET %d", socket);
            close(socket);
        }
    }
}

void enviar_a_instancia(mensaje_t* mensaje){

	char *bytes;
	int numero_bytes;

	int socket_instancia = seleccionar_socket();

	switch(mensaje->header){
		case H_GET:
			numero_bytes = serializar_mensaje(*mensaje, &bytes);
			enviar_mensaje(socket_instancia, bytes, numero_bytes);
			break;

		case H_SET:
			numero_bytes = serializar_mensaje(*mensaje, &bytes);
			enviar_mensaje(socket_instancia, bytes, numero_bytes);
			break;

		case H_STORE:
			numero_bytes = serializar_mensaje(*mensaje, &bytes);
			enviar_mensaje(socket_instancia, bytes, numero_bytes);
			break;

		default:
			fprintf(stderr, "No pude interpretar \n");
			exit(EXIT_FAILURE);
	}

	destruir_mensaje(&mensaje);
	free(bytes);
			
	return;
}

// TODO: Logica para seleccionar la instancia correcta de acuerdo al algoritmo configurado
int seleccionar_socket() {
	return list_get(instancias, 0);
}

t_log* configure_logger() {
    return log_create("coordinador.log", "coordinador", true, LOG_LEVEL_INFO);
		
}

