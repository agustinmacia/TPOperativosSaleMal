#ifndef SOCKET_H_
#define SOCKET_H_

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
#include "protocolo.h"

#define BACKLOG 10

typedef char header_t;

t_log * logger;

// ------------ FUNCIONES GENERALES -------------
int crear_socket();
int conectar_a(int server_socket, char *ip, char* puerto);
int escuchar_en(int socket, char* puerto);
int aceptar_conexion(int socket, struct sockaddr_in * direccion_cliente);
void enviar_mensaje(int socket, char* mensaje, int largo);
mensaje_t* recv_mensaje(int socket);
void enviar_notificacion(int socket, header_t notificacion);
header_t recv_notificacion(int socket);
int agregar_fd(int fd, fd_set *master);
int obtener_fd_mas_grande(int fd, int fd_mas_grande);
void *internet_addr(struct sockaddr *sa);
void descripcion_conexion_exitosa(struct sockaddr_storage direccion_cliente, int fd);
void obtener_informacion(int socket_servidor, void (*manejador)());

//------------- MANEJO DE ERRORES ---------------
void verificar_error(int fd, char* error);
void verificar_error_igual_a(int value, int fd, char* error);
void verificar_error_distinto_de(int value, int fd, char* error);
void salida_con_error(int socket, char* error_msg, void * buffer);
void exit_gracefully(int return_nr);


#endif /* SOCKET_H_ */
