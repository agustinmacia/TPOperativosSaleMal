#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>

typedef char header_t;

typedef struct {
	int tamanio_valor;
	char *valor;
} payload_set_t;

typedef struct {
	header_t header;
	char tamanio_clave;
	char *clave;
	payload_set_t *payload_set;
} mensaje_t;

// CODIGOS DE ERROR
#define EXITO 0
#define ERROR_MALLOC -1
#define ERROR_HEADER_INVALIDO -2
#define ERROR_CLAVE_MUY_GRANDE -3
#define ERROR_PUNTERO_A_NULL -4

// Esto es lo que pasa cuando sos rata y no queres gastar un byte mas,
// no hagan esto en casa

// Identificadores para handshake
#define H_SOY_COORDINADOR		0x00
#define H_SOY_PLANIFICADOR		0x01
#define H_SOY_ESI				0x02
#define H_SOY_INSTANCIA			0x03
#define H_SOY_CONSOLA			0x04

// Instrucciones, se propagan en todo el sistema
#define H_GET				0x05
#define H_SET				0x06
#define H_STORE				0x07

// Instrucciones de la consola del planificador
#define H_PAUSE				0x08
#define H_CONTINUE			0x09
#define H_LOCK				0x0A
#define H_UNLOCK			0x0B
#define H_LIST				0x0C
#define H_KILL				0x0D
#define H_STATUS			0x0E
#define H_DEADLOCK			0x0F

#define H_EXITO	0x10

// Notificaciones del planificador-coordinador
#define H_ERROR_CLAVE_MUY_GRANDE 0x11
#define H_ERROR_CLAVE_NO_IDENTIFICADA 0x12
#define H_ERROR_DE_COMUNICACION 0x13
#define H_ERROR_CLAVE_INEXISTENTE 0x14

// Instrucciones para handshake de esi
#define H_COMENZAR 			0x15
#define H_SIGUIENTE			0x16
#define H_LEER				0x17
#define H_MATAR				0x18
#define H_TERMINADO			0x19

/**
 * @DESC: Crea un nuevo mensaje con header
 * @RETURNS: La direccion del mensaje alocado va a quedar en el puntero que le paso por parametro
 * 	-	EXITO: 0
 * 	-	ERROR_MALLOC: -1
 * 	-	ERROR_HEADER_INVALIDO: -2
*/
int crear_mensaje(header_t header, mensaje_t **p);

/**
 * @DESC: Crea un mensaje con la clave que le paso por parametro
 * @RETURNS: La direccion del mensaje alocado va a quedar en el puntero que le paso por parametro
 * 	-	EXITO: 0
 * 	-	ERROR_MALLOC: -1
 * 	-	ERROR_HEADER_INVALIDO: -2
 * 	-	ERROR_CLAVE_MUY_GRANDE: -3
*/
int crear_mensaje_con_clave(header_t header, char* clave, mensaje_t **mensaje);

/**
 * @DESC: Destruye un mensaje allocado con crear_mensaje(), aparte nullea el
 * puntero que le paso por parametro
*/
void destruir_mensaje(mensaje_t **p);

/**
 * @DESC: Settea la clave en el mensaje que le pase por parametro.
 * IMPORTANTE: Seguramente antes de usar esta funcion ya usaste un monton de veces
 * &mensaje en el segundo parametro. NO LO HAGAS, no es un **!!!. Putie como 2 horas con esto.
 * @RETURNS: La direccion del mensaje alocado va a quedar en el puntero que le paso por parametro
 * 	-	EXITO: 0
 * 	-	ERROR_HEADER_INVALIDO: -2
 * 	-	ERROR_CLAVE_MUY_GRANDE: -3
*/
int set_clave(char* clave, mensaje_t *mensaje);

/**
 * @DESC: Settea el valor en el mensaje que le pase por parametro
 * @RETURNS: La direccion del mensaje alocado va a quedar en el puntero que le paso por parametro
 * 	-	EXITO: 0
 * 	-	ERROR_MALLOC: -1	(malloc cuando trato de instanciar el payload)
 * 	-	ERROR_HEADER_INVALIDO: -2
 * 	-	ERROR_CLAVE_MUY_GRANDE: -3
*/
int set_valor(char* valor, mensaje_t *mensaje);

/**
 * @DESC: Simplemente hace eso, devuelve el valor, y ademas aumenta un buffer
 * por ese valor
 */
int retornar_aumentando_buffer(int valor, int *buffer);

int serializar_mensaje(mensaje_t mensaje, char **return_pointer);

#endif /* PROTOCOLO_H_ */
