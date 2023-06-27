#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <pthread.h>

#define PLUGIN_DIR	"./plugins/"


typedef struct {
	char *name;
	pthread_t thread;
	int (*parse)(char*, char**);
	void (*init)(int, char*, ...);
	void (*process)(void);
	// FUTURE: method that returns supported commnads (for autocomplete perhaps)
	//		   method that requests something from ncist
} plugin_t;

int plugin_load_all(void);
int plugin_load(char *name);
int plugin_list(void);
void plugin_rx(int id, char *str, ...);

#endif
