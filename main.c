#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>

#include "ncist.h"
#include "main.h"
#include "plugin.h"

// TODO: ability for colors in print function
// TODO: a bug causes the plugin callback to write garbage to the cmdline
// TODO: printing multiple lines at once breaks the window
// TODO: command history
// TODO: seperate ui and plugin code to different files
// TODO: window should be mutex
// TODO: the output is not in any sort of buffer, so no scrolling/redrawing of any kind
// TODO: resize handler
// TODO: be C89/C99 compatible?
// TODO: iron out the full plugin API? there could be multiple types of plugins (some just req->resp)
// TODO: adding something to a text file in a few key strokes (like log or idea)
// FUTURE: some sort of date/time keeping for the program is needed for sure
// FUTURE: plugins planned: log, habit/task tracker, website manager (with solsaz), markdown... thingy?
// FUTURE: also text file viewer (with my own format)
// FUTURE: more website APIs, github, email, etc
// FUTURE: notifiation receiver
// FUTURE: other windows, more than just text etc


extern plugin_t plugins[];
extern int plugin_count;

int outline;
char outbuf[0x8000]; // 32K
char cmdbuf[256];

WINDOW *wincmd;
WINDOW *winout;
pthread_mutex_t outlock;


void init()
{
	int rc;

	rc = pthread_mutex_init(&outlock, NULL);
	if (rc)
	{
		printf("Can't init mutex\n");
		exit(-1);
	}

	outline = 0;
	initscr();
	cbreak();
	noecho();
	refresh();
}

int main(int argc, char *argv[])
{
	init();


	// Ready screen
	winout = newwin(LINES-CMD_LINES, COLS, 0, 0);
	wincmd = newwin(CMD_LINES, COLS, LINES-CMD_LINES, 0);
	box(winout, 0 , 0);
	box(wincmd, 0 , 0);

	plugin_load_all();

	wrefresh(winout);
	wrefresh(wincmd);

	char c;
	int cmdlen;

	cmdlen = 0;
	cmdbuf[0] = 0;

	while(c = getch())
	{
		switch (c)
		{
			case LF:
				cmdparse(cmdbuf);
				cmdlen = 0;
				cmdbuf[0] = 0;
				break;
			case BS:
				if (cmdlen <= 0) break;
				cmdbuf[--cmdlen] = 0;
				break;
			default:
				if (c < 0x20 || c >= 0x7F) break;
				cmdbuf[cmdlen] = c;
				cmdbuf[++cmdlen] = 0;
				break;
		}

		winclear(wincmd);
		winprint(wincmd, 0, cmdbuf);

		wrefresh(winout);
		wrefresh(wincmd);
	}
}

void log_msg(char *str, ...)
{
	va_list ap;
	char buf[256];

	va_start(ap, str);
	vsprintf(buf, str, ap);
	winappend(winout, buf);
	va_end(ap);
}

void log_error(char *str)
{
	char buf[256];
	strcpy(buf, "ERROR: ");
	strcat(buf, str);
	winappend(winout, buf);
}

void prog_exit()
{
    endwin();
	pthread_mutex_destroy(&outlock);
	// TODO close all plugins and join threads
	// and dlclose() all
	// close plugins()
	// close terminal()
	// save shit()
	exit(0);
}

void cmdparse(char *str)
{
	char *args;
	char *outstr;
	char buf[256];

	int i;
	for (i = 0; i < plugin_count; i++)
		plugins[i].parse(str, &outstr);

	// Non-plugin commands. Keep to a minimum
	if CMD(str, "clear")
	{
		winclear(winout);
		outline = 0;
	}
	else if (CMD(str, "exit") || CMD(str, "qq"))
	{
		prog_exit();
	}
	else if CMD(str, "plug all")
	{
		plugin_load_all();
	}
	else if CMD(str, "plug ls")
	{
		plugin_list();
	}
}

void winclear(WINDOW *win)
{
	wclear(win);
	box(win, 0, 0);
}

void winappend(WINDOW *win, char *str)
{
	winprint(win, outline++, str);
}

void winprint(WINDOW *win, int line, char *str)
{
	mvwaddstr(win, line+1, 2, str);
	wrefresh(win);
}