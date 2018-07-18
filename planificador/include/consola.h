#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <readline/readline.h>
#include <commons/string.h>
#include <stdio.h>
#include <pthread.h>

#define COMMANDS_SIZE 9

typedef enum {
	PAUSE,
	CONTINUE,
	LOCK,
	UNLOCK,
	LIST,
	KILL,
	STATUS,
	DEADLOCK,
	QUIT,
	INVALID_TOKEN = -1
} Token;

typedef struct thread_args_t {
	pthread_mutex_t lock;
	bool *debe_continuar;
} thread_args_t;

const char* commands[COMMANDS_SIZE];
void* consola_thread(void *vargp);

#endif /* CONSOLA_H_ */
