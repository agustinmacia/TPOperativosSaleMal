#include "protocolo.h"

bool header_valido(header_t header) {
	return header == H_GET || header == H_SET || header == H_STORE ||
			header == H_SOY_COORDINADOR || header == H_SOY_PLANIFICADOR || header == H_SOY_ESI || 
			header == H_SOY_INSTANCIA || header == H_SOY_CONSOLA || header == H_TERMINADO;
}

int crear_mensaje(header_t header, mensaje_t **mensaje) {
	if (!header_valido(header)) {
		return ERROR_HEADER_INVALIDO;
	}

	*mensaje = (mensaje_t *) malloc(sizeof(mensaje_t));
	// malloc devuelve NULL si no se pudo alocar memoria.
	if(!(*mensaje)) {
		return ERROR_MALLOC;
	}
	
	(*mensaje)->header = header;
	(*mensaje)->tamanio_clave = 0;
	(*mensaje)->clave = NULL;
	(*mensaje)->payload_set = NULL;

	return EXITO;
}

void destruir_mensaje(mensaje_t **mensaje) {
	// Se que vas a leer este bloque de codigo y vas a decir "En que mierda
	// estaba pensando este pelotudo? hay como 40 ifs". Si, tenes razon, pero
	// asi te garantizas que:
	// 1. Siempre hagas cleanup correcto.
	// 2. Nunca segfaultee (esto es mas importante).
	if(mensaje) {
		if(*mensaje) {
			if((*mensaje)->clave) {
				free((*mensaje)->clave);
			}
			if((*mensaje)->payload_set) {
				if((*mensaje)->payload_set->valor) {
					free((*mensaje)->payload_set->valor);
				}
				free((*mensaje)->payload_set);
			}
		}

		free(*mensaje);
		// Nulleo el puntero para que sea mas facil revisar errores despues,
		// aparte esto: https://stackoverflow.com/a/16267025 me dio miedo.
		*mensaje = NULL;
	}
}

int set_clave(char* clave, mensaje_t *mensaje) {
	int largo = strlen(clave);
	
	// Valor de clave maximo es de 40 caracteres
	if (largo > 40) {
		return ERROR_CLAVE_MUY_GRANDE;
	}

	if (!header_valido(mensaje->header)) {
		return ERROR_HEADER_INVALIDO;
	}

	// TODO: Definir si en el tamanio va el char '\0' o no
	mensaje->tamanio_clave = largo + 1;
	mensaje->clave = strdup(clave);

	return EXITO;
}


int set_valor(char* valor, mensaje_t *mensaje) {
	int tamanio_valor = strlen(valor) + 1;

	if (mensaje->header != H_SET) {
		return ERROR_HEADER_INVALIDO;
	}

	payload_set_t *payload = (payload_set_t *) malloc(sizeof(payload_set_t));

	if (!payload) {
		return ERROR_MALLOC;
	}

	payload->valor = (char *) malloc(tamanio_valor);

	if (!(payload->valor)) {
		return ERROR_MALLOC;
	}

	// Dos cosas horribles: primero, todavia esto no esta testeado, segundo
	// no estoy revisando overflows ni en pedo (creo).
	memcpy(payload->valor, valor, tamanio_valor);

	payload->tamanio_valor = tamanio_valor;

	mensaje->payload_set = payload;

	return EXITO;
}

int crear_mensaje_con_clave(header_t header, char* clave, mensaje_t **mensaje) {
	int rv = crear_mensaje(header, mensaje);

	// Burbujeo el error para arriba. Quien te conoce excepcion?
	if (rv == ERROR_MALLOC || rv == ERROR_HEADER_INVALIDO) {
		return rv;
	}

	rv = set_clave(clave, *mensaje);

	if (rv == ERROR_CLAVE_MUY_GRANDE) {
		return rv;
	}

	return 0;
}

// Sera muy overkill crear_mensaje_con_clave_y_valor ?
// Haran falta getters?

int retornar_aumentando_buffer(int valor, int *buffer) {
	(*buffer) += valor;
	return valor;
}

char* copiar_memoria_devolviendo_offset(char* dest, void* src, int tamanio) {
	memcpy(dest, src, tamanio);
	return dest + tamanio;
}

/**
 * @DESC: Pasandole un mensaje valido se lo serializa.
 * @RETURNS: La cantidad total de bytes que se serializaron y el segundo parametro (return_pointer) va a guardar
 * la direccion de memoria con el mensaje serializado. Si le paso NULL ahi, la funcion devuelve 0 y no serializa nada
 */
int serializar_mensaje(mensaje_t mensaje, char **return_pointer) {

	if (!return_pointer) {
		return 0;
	}

	// Que clase de magia negra hace que ande con esto??? (When you see commits anteriores you'll shit bricks)
	int tamanio_total = 0;
	int	tamanio_header,
		tamanio_tamanio_clave,
		tamanio_clave,
		tamanio_tamanio_valor,
		tamanio_valor = 0;

	tamanio_header = retornar_aumentando_buffer(sizeof(mensaje.header), &tamanio_total);
	tamanio_tamanio_clave = retornar_aumentando_buffer(sizeof(mensaje.tamanio_clave), &tamanio_total);
	tamanio_clave = retornar_aumentando_buffer(mensaje.tamanio_clave, &tamanio_total);

	if (mensaje.header == H_SET) {
		payload_set_t payload = *mensaje.payload_set;
		tamanio_tamanio_valor = retornar_aumentando_buffer(sizeof(payload.tamanio_valor), &tamanio_total);
		tamanio_valor = retornar_aumentando_buffer(payload.tamanio_valor, &tamanio_total);
	}

	char* serializado = (char*) malloc(tamanio_total);
	char* offset = serializado;

	offset = copiar_memoria_devolviendo_offset(offset, &(mensaje.header), tamanio_header);
	offset = copiar_memoria_devolviendo_offset(offset, &(mensaje.tamanio_clave), tamanio_tamanio_clave);
	offset = copiar_memoria_devolviendo_offset(offset, mensaje.clave, tamanio_clave);

	if (mensaje.header == H_SET) {
		payload_set_t payload = *mensaje.payload_set;
		offset = copiar_memoria_devolviendo_offset(offset, &(payload.tamanio_valor), tamanio_tamanio_valor);
		copiar_memoria_devolviendo_offset(offset, payload.valor, tamanio_valor);
	}

	*return_pointer = serializado;

	return tamanio_total;
}