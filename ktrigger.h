#ifndef KTRIGGER_H
#define KTRIGGER_H

#include <stdbool.h>

struct config_s {
	char*	dir;
	char*	command;
	bool	loop;
	int	filter;
};

void usage(char*);
void init_config(struct config_s*);
void parse_cmd(struct config_s*, int, char**);
int run_trigger(char*, char*, int);

#endif	/* KTRIGGER_H */
