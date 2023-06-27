#include <stdarg.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <pthread.h>
#include <ncurses.h>


#include "main.h"
#include "plugin.h"

extern pthread_mutex_t outlock;
extern WINDOW *winout;


plugin_t plugins[64];
int plugin_count = 0;

int plugin_load_all()
{
	DIR *dir;
	struct dirent *ent;

	dir = opendir("./plugins");
	if (!dir)
	{
		log_error("Can't open plugins dir.");
		return -1;
	}

	while (ent = readdir(dir))
	{
		if (ent->d_type != DT_REG) continue;

		plugin_load(ent->d_name);
	}

	return 0;
}

int plugin_load(char *file)
{
	void *handle;
	plugin_t *p;
	char buf[128];

	strcpy(buf, PLUGIN_DIR);
	strcat(buf, file);

	handle = dlopen(buf, RTLD_LAZY);
	if (!handle)
	{
		log_error(dlerror());
		return -1;
	}

	p = &plugins[plugin_count];

	p->name = dlsym(handle, "_name");
	p->parse = dlsym(handle, "cmdparse");
	p->init = dlsym(handle, "_plugin_init");
	p->process = dlsym(handle, "process");

	p->init(plugin_count, plugin_rx);
	pthread_create(&p->thread, NULL, p->process, NULL);
	log_msg("Plugin loaded: %s\t%d", p->name, p->thread);

	plugin_count++;

	return 0;
}

int plugin_list(void)
{
	int i;

	log_msg("Loaded Plugins:");
	for (i = 0; i < plugin_count; i++)
	{
		log_msg(plugins[i].name);
	}
}

void plugin_rx(int id, char *str, ...)
{
	va_list ap;
	char buf[8192];
	int off;

	va_start(ap, str);
	off = sprintf(buf, "[%s] ", plugins[id].name);
	vsprintf(&buf[off], str, ap);
	pthread_mutex_lock(&outlock);
	winappend(winout, buf);
	pthread_mutex_unlock(&outlock);
	va_end(ap);
}