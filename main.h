#ifndef _NCIST_H
#define _NCIST_H

#define	CMD_LINES	(1+2)

#include <ncurses.h>

void init(void);

// TODO: these two should be unified in a print method
void log_msg(char *str, ...);
void log_error(char *str);
void cmdparse(char *str);
void winclear(WINDOW *win);
void winappend(WINDOW *win, char *str);
void winprint(WINDOW *win, int line, char *str);

#endif