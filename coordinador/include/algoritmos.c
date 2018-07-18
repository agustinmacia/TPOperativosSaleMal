#include "algoritmos.h"

const char* algoritmos_distribucion[3] = {
		"LSU",
		"EL",
		"KE"
};

/**
 * @DESC: Toma la representacion como string de un algoritmo de distribucion
 * y devuelve la representacion como enum. En caso de que no sea un algo valido
 * devuelve -1.
 *
 * @PARAMS:
 * 		str - representacion como cadena del algo.
 */
const Algoritmo_distribucion algoritmo_from_string(const char* str) {
	for(int i=LSU; i <= KE; i++) {
		const char* algoritmo = algoritmos_distribucion[i];

		if (string_equals_ignore_case(str, algoritmo)) {
			return i;
		}

	}
	return -1;
}
