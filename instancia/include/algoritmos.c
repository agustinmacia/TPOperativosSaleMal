#include "algoritmos.h"

const char* algoritmos_reemplazo[3] = {
		"CIRC",
		"LRU",
		"BSU"
};

/**
 * @DESC: Toma la representacion como string de un algoritmo de distribucion
 * y devuelve la representacion como enum. En caso de que no sea un algo valido
 * devuelve -1.
 *
 * @PARAMS:
 * 		str - representacion como cadena del algo.
 */
const Algoritmo_reemplazo algoritmo_instancia_from_string(const char* str) {
	for(int i=CIRC; i <= BSU; i++) {
		const char* algoritmo = algoritmos_reemplazo[i];

		if (string_equals_ignore_case(str, algoritmo)) {
			return i;
		}

	}
	return -1;
}
