#include "consola.h"

const char* commands[COMMANDS_SIZE] = {
	"pause",
	"continue",
	"lock",
	"unlock",
	"list",
	"kill",
	"status",
	"deadlock",
	"quit"
};

void manejar_lock() {
	char *key = strtok(NULL, " ");
	char *id_as_string = strtok(NULL, "");

	if (!key || !id_as_string) {
		printf("uso: lock <key> <id>\n");
		return;
	}

	int id = atoi(id_as_string);

	printf("Aca es donde llamaria la funcion del planificador correspondiente.\n");
}

void manejar_unlock() {
	char *key = strtok(NULL, "");

	if (!key) {
		printf("uso: unlock <key>\n");
		return;
	}

	printf("Aca es donde llamaria la funcion del planificador correspondiente.\n");
}

void manejar_lista() {
	char *resource = strtok(NULL, "");

	if (!resource) {
		printf("uso: unlock <resource>\n");
		return;
	}

	printf("Aca es donde llamaria la funcion del planificador correspondiente.\n");
}

void manejar_kill() {
	char* id_as_string = strtok(NULL, "");

	if (!id_as_string) {
		printf("uso: kill <id>\n");
		return;
	}

	int id = atoi(id_as_string);

	printf("Aca es donde llamaria la funcion del planificador correspondiente.\n");
}

void manejar_estado() {
	char *key = strtok(NULL, "");

	if(!key) {
		printf("uso: status <key>\n");
		return;
	}

	printf("Aca es donde llamaria la funcion del planificador correspondiente.\n");
}

void* consola_thread(void *vargp) {
	thread_args_t *args = vargp;

	pthread_mutex_lock(&args->lock);
	while(*args->debe_continuar) {
		pthread_mutex_unlock(&args->lock);
		char *line = readline("> ");

		if (!line) {
			printf("Bye!\n");
			break;
		}

		char *command = strtok(line, " ");
    if (!command) continue;

		Token command_token = INVALID_TOKEN;

		for (int i=PAUSE; i <= QUIT; i++) {
			if(string_equals_ignore_case(command, commands[i])) {
				// Hack horrible aprovechando que un enum en c es en realidad
				// un numero entero, si a alguien se le ocurre una mejor manera
				// esta invitado a arreglar esto.
				command_token = i;
			}
		}

		switch (command_token) {
		case PAUSE:
			// Handle Pause
			break;
		case CONTINUE:
			// Handle Continue
			break;
		case LOCK:
			manejar_lock();
			break;
		case UNLOCK:
			manejar_unlock();
			break;
		case LIST:
			manejar_lista();
			break;
		case KILL:
			manejar_kill();
			break;
		case STATUS:
			manejar_estado();
			break;
		case DEADLOCK:
			// Handle Deadlock
			break;
		case QUIT:
			pthread_mutex_lock(&args->lock);
			*args->debe_continuar = false;
			pthread_mutex_unlock(&args->lock);
			break;
		case INVALID_TOKEN:
			printf("Invalid command\n");
			break;
		default:
			printf("Not implemented yet.\n");
			break;
		}

		free(line);
		pthread_mutex_lock(&args->lock);
	}
	pthread_mutex_unlock(&args->lock);

	printf("Bye!\n");
	return 0;
}
