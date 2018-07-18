#include "include/esi.h"

int main(int argc, char **argv){
	
	Esi_config CONFIG = iniciar_config("esi.cfg");
	configure_logger();

	if (argc < 2) {
		log_error(logger, "Uso: ./esi <script>");
		log_info(logger, "No se esta haciendo el cleanup adecuado");
		exit(-1);
	}

	if(access(argv[1], F_OK) < 0) {
		log_error(logger, "El script \"%s\" no existe", argv[1]);
		log_info(logger, "No se esta haciendo el cleanup adecuado");
		exit(-1);
	}
	
	int socket_coordinador = crear_socket();
	conectar_a(socket_coordinador, CONFIG.ip_coordinador, CONFIG.puerto_coordinador);
	enviar_notificacion(socket_coordinador, H_SOY_ESI);

	int socket_planificador = crear_socket();
	conectar_a(socket_planificador, CONFIG.ip_planificador, CONFIG.puerto_planificador);
	enviar_notificacion(socket_planificador, H_SOY_ESI);

	header_t accion = recv_notificacion(socket_planificador);

	if (accion == H_COMENZAR) {
		leer_archivo(argv[1], socket_planificador, socket_coordinador);

	} else {
		log_info(logger, "Accion recibida invalida");							
	}

	
	return 0;

}