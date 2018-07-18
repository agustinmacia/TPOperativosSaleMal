#ifndef ALGORITMOS_INSTANCIA_H_
#define ALGORITMOS_INSTANCIA_H_

typedef enum {
	CIRC,
	LRU,
	BSU
} Algoritmo_reemplazo;

const Algoritmo_reemplazo algoritmo_instancia_from_string(const char* str);

#endif /* ALGORITMOS_INSTANCIA_H_ */
