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

static u_int get_event(const char*);
static short get_filter(const char*);
static void usage();
void parse_cmd(struct ktrigger*, int, char**);
int run_trigger(struct ktrigger*);

#endif	/* KTRIGGER_H */
