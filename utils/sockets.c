#include "sockets.h"

#define EXIT_SUCCESS 0

// ------------ FUNCIONES GENERALES -------------

int crear_socket() {

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	verificar_error_igual_a(1, server_socket, "Error creando el socket");

	int yes = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	return server_socket;
}

int conectar_a(int server_socket, char *ip, char* puerto){

	log_info(logger, " ... Conectando al servidor ... ");

	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int estado_conexion = connect(server_socket, server_info->ai_addr,
			server_info->ai_addrlen);

	verificar_error_igual_a(-1, server_socket, "Error de socket");
	verificar_error_igual_a(-1, estado_conexion, "Error de conexión con el servidor");

	freeaddrinfo(server_info);

	log_info(logger, " Conexión exitosa ");

	return server_socket;
}

int escuchar_en(int listener, char* puerto){

	log_info(logger, " ... Escuchando clientes ... ");

	struct addrinfo hints;
	struct addrinfo *ai;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int getaddrinfo_error = getaddrinfo(NULL, puerto, &hints, &ai);
	
	int bindrv = bind(listener, ai->ai_addr, ai->ai_addrlen);
	verificar_error_igual_a(1, bindrv, "Error bindeando");

	freeaddrinfo(ai);

	int listenrv = listen(listener, 10); 
	verificar_error_igual_a(1, listenrv, "Error en el listen");

	return EXIT_SUCCESS;

}

int aceptar_conexion(int socket, struct sockaddr_in * direccion_cliente){
	socklen_t addr_length;
	addr_length = sizeof(direccion_cliente);
	return accept(socket, (void*) &direccion_cliente, &addr_length);
}

void enviar_mensaje(int socket, char* mensaje, int largo){
	int resultado = send(socket, mensaje, largo, 0);
    verificar_error_igual_a(-1, resultado, "No se pudo enviar el mensaje");
}

mensaje_t* recv_mensaje(int socket){

	header_t header;
	int resultado_header = recv(socket, &header, sizeof(header_t), MSG_WAITALL);
	verificar_error_igual_a(-1, resultado_header, "Error al recibir header");
	
	mensaje_t *mensaje;
	
	if ( header == H_SOY_COORDINADOR || 
				header == H_SOY_PLANIFICADOR ||
				header == H_SOY_ESI ||
				header == H_SOY_INSTANCIA ||
				header == H_SOY_CONSOLA || 
				header == H_TERMINADO ){
		crear_mensaje(header, &mensaje);
		return mensaje;
	}

	char tamanio_clave;
	int resultado_tamanio_clave = recv(socket, &tamanio_clave, sizeof(tamanio_clave), MSG_WAITALL);
	verificar_error_igual_a(-1, resultado_tamanio_clave, "Error al recibir tamanio_clave");
		
	char *clave = malloc(tamanio_clave);
	int resultado_clave = recv(socket, clave, tamanio_clave, MSG_WAITALL);
	verificar_error_igual_a(-1, resultado_clave, "Error al recibir el clave");
		
	crear_mensaje_con_clave(header, clave, &mensaje);
		
	if (header == H_SET){
		int tamanio_valor;
		int resultado_tamanio_valor = recv(socket, &tamanio_valor, sizeof(tamanio_valor), MSG_WAITALL);
		verificar_error_igual_a(-1, resultado_tamanio_valor, "Error al recibir tamanio_valor");

		char *valor = malloc(tamanio_valor);
		int resultado_valor = recv(socket, valor, tamanio_valor, MSG_WAITALL);
		verificar_error_igual_a(-1, resultado_valor, "Error al recibir el valor");

		set_valor(valor, mensaje);
	} 
	
	
	return mensaje;
}

void enviar_notificacion(int socket, header_t notificacion){
	int resultado = send(socket, &notificacion, sizeof(notificacion), 0);
	verificar_error(resultado, "No se pudo enviar la notificacion");
}

header_t recv_notificacion(int socket){
	header_t notificacion;
	int resultado = recv(socket, &notificacion, sizeof(notificacion), MSG_WAITALL);
	verificar_error(resultado, "Error al recibir la notificacion");
	return notificacion;
}

int agregar_fd(int fd, fd_set *master){
	FD_SET(fd, master); // Agrego al nuevo File Descriptor al master

	return EXIT_SUCCESS;
}

int obtener_fd_mas_grande(int fd, int fd_mas_grande){
	if (fd > fd_mas_grande) {
		fd_mas_grande = fd; // Es el nuevo mas grande.		
	}

	return fd_mas_grande;
}

/**
 * Obtiene la sockaddr, sea IPV4 o IPV6
 */
void *internet_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

void descripcion_conexion_exitosa(struct sockaddr_storage cliente_addr, int fd){

	char remoteIP[INET_ADDRSTRLEN];		// Vamos por IPv4

	// la address de mi cliente como un struct,
	void * addr_as_n = internet_addr( (struct sockaddr *) &cliente_addr);

	char *ip_cliente_as_string = inet_ntop( cliente_addr.ss_family, addr_as_n, remoteIP, INET_ADDRSTRLEN);

	log_info(logger, "Logre conectarme a %s en el socket %d", ip_cliente_as_string, fd);

}

void obtener_informacion(int server_socket, void (*manejador)()){

	struct sockaddr_storage direccion_cliente;
	memset(&direccion_cliente, 0, sizeof(direccion_cliente));

	int nuevoFD;	// Socket recien aceptado
	
	// temp file descriptor list for select()
	
	fd_set master;	// Esta es la referencia a todos los sockets clientes
	fd_set readFDS;	// Estos son los que uso para select()

	FD_ZERO(&master);
	FD_ZERO(&readFDS);
	
	// Lo agrego al pool de sockets
	FD_SET(server_socket, &master);

	// Tiene el valor del socket cuyo fd es el mayor.
	int fd_mas_grande = server_socket;
	
	while (true) {
		readFDS = master;

		int selectrv = select(fd_mas_grande + 1, &readFDS,
				NULL,	// sockets de escritura, no se usan
				NULL,	// sockets de excepciones, no se usan
				NULL);	// timeout, no se usa.

		verificar_error_igual_a(-1, selectrv, "select");

		// Iteramos sobre los sockets abiertos buscando data para leer
		for (int socket_existente = 0; socket_existente <= fd_mas_grande; socket_existente++) {

			// El socket tenia informacion para leer (esta en readFDS).
			if (FD_ISSET(socket_existente, &readFDS)) {

				// Se fija si el socket que analizo es el mismo que el que estoy escuchando ahora
				if (socket_existente == server_socket) {
					nuevoFD = aceptar_conexion(server_socket, (struct sockaddr *) &direccion_cliente);

					if (nuevoFD == -1) {
						log_error(logger, "accept pecheo");
					} else {
						agregar_fd(nuevoFD, &master);
						fd_mas_grande = obtener_fd_mas_grande(nuevoFD, fd_mas_grande);
					
						descripcion_conexion_exitosa(direccion_cliente, nuevoFD);
							
					}
				} else {	// Manejo informacion de un cliente
					manejador(socket_existente);
					FD_CLR(socket_existente, &master);
				}
			}
		}

	}
}

//------------- MANEJO DE ERRORES ---------------

void verificar_error(int fd, char* error) {
	if (fd == -1) {
		close(fd);
		salida_con_error(fd, error, NULL);
	}
}

void verificar_error_igual_a(int value, int fd, char* error){
	if (fd == value) {
		close(fd);
		salida_con_error(fd, error, NULL);
	}
}

void verificar_error_distinto_de(int value, int fd, char* error){
	if (fd != value) {
		//close(fd);
		salida_con_error(fd, error, NULL);
	}
}

void salida_con_error(int socket, char* error_msg, void * buffer){
	if (buffer != NULL){
		free(buffer);
	}
	log_error(logger, error_msg);
	exit_gracefully(1);
} 

void exit_gracefully(int return_nr){
	log_destroy(logger);
	exit(return_nr);
}
