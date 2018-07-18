#include "include/instancia.h"

int main() {

	Instancia_config CONFIG = iniciar_config("instancia.cfg");
	configure_logger();

	int socket_coordinador = crear_socket();
	conectar_a(socket_coordinador, CONFIG.ip_coordinador, CONFIG.puerto_coordinador);
	enviar_notificacion(socket_coordinador, H_SOY_INSTANCIA);

	manejador(socket_coordinador);

	return 0;

}
