#ifndef KTRIGGER_H
#define KTRIGGER_H

struct config {
	char*	dir;
	char*	command;
	int	loop;
	short	filter;
};

static void usage();
void parse_cmd(struct config*, int, char**);
int run_trigger(char*, char*, short);

#endif	/* KTRIGGER_H */
