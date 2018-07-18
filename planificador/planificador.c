

#include "include/planificador.h"

int main() {

	bool continuar = true;

	// Inicializo el thread de consola
	thread_args_t *args = malloc(sizeof(struct thread_args_t));
	args->debe_continuar = &continuar;
	pthread_mutex_init(&args->lock, NULL);
	pthread_t thread_consola;
	pthread_create(&thread_consola, NULL, consola_thread, args);

	// Inicializo el thread del planificador
	pthread_t thread_server;
	pthread_create(&thread_server, NULL, manejar_sockets, NULL);

	pthread_mutex_lock(&args->lock);
	while(continuar) {
		pthread_mutex_unlock(&args->lock);
		sleep(1);
		pthread_mutex_lock(&args->lock);
	}
	pthread_mutex_unlock(&args->lock);

	return 0;
}

// int main() {
// 	// Es muy dificil revisar leaks en los tests, me volvi puto haciendolo.
// 	listos = list_create();
// 	bloqueados = list_create();
// 	info_esi *esi = crear_esi(1, 1);
// 	esi->claves = list_create();
// 	destruir_esi(&esi);
// 	list_destroy(listos);
// 	list_destroy(bloqueados);
// }


