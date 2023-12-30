#ifndef KTRIGGER_H
#define KTRIGGER_H

#include <sys/types.h>

struct ktrigger {
	char*	dir;
	char*	cmd;
	int	loop;
	short	filter;
	u_int	fflags;
};

static u_int get_event(char*);
static short get_filter(char*);
static void usage();
void parse_cmd(struct ktrigger*, int, char**);
int run_trigger(struct ktrigger*);

#endif	/* KTRIGGER_H */
