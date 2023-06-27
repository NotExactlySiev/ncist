#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ncist.h"
#include "plug.h"

PLUGIN_PREAMBLE("Say")


int init()
{
    return 0;
}

int cmdparse(char *cmd)
{
    char *args;

    if CMD(cmd, "say")
	{
        callback(args);
        return 0;
	}

    return -1;
}

int process()
{
    return 0;
}