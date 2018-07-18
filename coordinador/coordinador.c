#include "include/coordinador.h"

int main(int argc, char **argv) {

	Coordinador_config CONFIG = iniciar_config("coordinador.cfg");
	logger = configure_logger();

	instancias = list_create();

	int socket = crear_socket();
	if (escuchar_en(socket, CONFIG.puerto) == 0) {
		obtener_informacion(socket, manejador);
	}

	return 0;
}

