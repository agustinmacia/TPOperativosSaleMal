#include "esi.h"

/**
 * @DESC: Va a buscar un archivo de configuracion en el *path* y devuelve una
 * estructura que contiene la informacion parseada. Ante error devuelve una
 * configuracion default.
 *
 * @PARAMS:
 * 		path - el archivo al que hay que ir a buscar la configuracion.
 */
Esi_config iniciar_config(char* path) {

	t_config *cfg = config_create(path);

	Esi_config config;

	if (!cfg) {

		config.ip_coordinador = "127.0.0.1";
		config.ip_planificador = "127.0.0.1";

		config.puerto_coordinador = "8080";
		config.puerto_planificador = "8081";

	} else {

		config.ip_coordinador = config_get_string_value(cfg, "ip_coordinardor");
		config.ip_planificador = config_get_string_value(cfg, "ip_planificador");

		config.puerto_coordinador = config_get_string_value(cfg, "puerto_coordinador");
		config.puerto_planificador = config_get_string_value(cfg, "puerto_planificador");
	}

	free(cfg);

	return config;
}


void configure_logger() {
	logger = log_create("esi.log", "esi", true, LOG_LEVEL_INFO);
}

int leer_archivo(char *archivo, int socket_planificador, int socket_coordinador) {
    FILE * fp;
    char * linea = NULL;
    size_t largo = 0;
    ssize_t estado_leer;

    fp = fopen(archivo, "r");
    if (fp == NULL) salida_con_error(NULL, "No se pudo abrir el archivo", fp);

    while ((estado_leer = getline(&linea, &largo, fp)) != -1) {
        t_esi_operacion parsed = parse(linea);
    	
    	if(parsed.valido){   
    		enviar_notificacion(socket_planificador, H_SIGUIENTE);

	        header_t accion = recv_notificacion(socket_planificador);

			if (accion == H_LEER) {
				ejecutar_operacion(parsed, socket_coordinador);

			} else if (accion == H_MATAR) {
				log_info(logger, "Realizar cleanup");							
				// Hago el cleanup

			} else {
				log_info(logger, "Accion recibida invalida");							
			}

			/* Aca puede ser que tengamos que recibir un mensaje del coordinador, 
			que le diga algo asi como "ok me llego bien el mensaje" o "volvemelo a 
			mandar, algo salio mal", para eso tenemos que hacer un checksum o algo asi */
	        

	        if (recv_notificacion(socket_coordinador) != H_EXITO) {
				log_info(logger, "Error al ejecutar la operacion");							
			}

			destruir_operacion(parsed);

		} else {
	        fprintf(stderr, "La linea <%s> no es valida\n", linea);
	        exit(EXIT_FAILURE);
    	}
    }

    enviar_notificacion(socket_coordinador, H_TERMINADO);
    enviar_notificacion(socket_planificador, H_TERMINADO);

    fclose(fp);
    if (linea)
        free(linea);

    return 0;
}

int ejecutar_operacion(t_esi_operacion parsed, int socket_coordinador){
	
	mensaje_t *mensaje;
    char *bytes;
	int numero_bytes;

	switch(parsed.keyword){
        case GET:
            crear_mensaje_con_clave(H_GET, parsed.argumentos.GET.clave, &mensaje);
            numero_bytes = serializar_mensaje(*mensaje, &bytes);
			enviar_mensaje(socket_coordinador, bytes, numero_bytes);

			printf("GET\tclave: <%s>\n", parsed.argumentos.GET.clave);
            break;

        case SET:
            crear_mensaje_con_clave(H_SET, parsed.argumentos.SET.clave, &mensaje);
            set_valor(parsed.argumentos.SET.valor, mensaje);
            numero_bytes = serializar_mensaje(*mensaje, &bytes);
            enviar_mensaje(socket_coordinador, bytes, numero_bytes);

            printf("SET\tclave: <%s>\tvalor: <%s>\n", parsed.argumentos.SET.clave, parsed.argumentos.SET.valor);
            break;

        case STORE:
            crear_mensaje_con_clave(H_STORE, parsed.argumentos.STORE.clave, &mensaje);
            numero_bytes = serializar_mensaje(*mensaje, &bytes);
            enviar_mensaje(socket_coordinador, bytes, numero_bytes);

            printf("STORE\tclave: <%s>\n", parsed.argumentos.STORE.clave);
            break;

        default:
            fprintf(stderr, "No pude interpretar \n");
            exit(EXIT_FAILURE);
    }

    destruir_mensaje(&mensaje);
    free(bytes);
       
    return EXITO;
}