#ifndef ALGORITMOS_H_
#define ALGORITMOS_H_

#include<commons/string.h>

typedef enum {
	LSU,
	EL,
	KE
} Algoritmo_distribucion;

const Algoritmo_distribucion algoritmo_from_string(const char* str);

#endif /* ALGORITMOS_H_ */
