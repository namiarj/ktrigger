#include <sys/event.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ktrigger.h"

void
usage(char *program_name)
{
	char *usage_msg = "usage: %s [-l] dir command\n";
	printf(usage_msg, program_name);
	exit(1);
}

void
init_config(struct config *config)
{
	config->loop = false;
	config->filter = EVFILT_VNODE;
}

void
parse_cmd(struct config *config, int argc, char **argv) 
{
	if (argc < 3)
		usage(argv[0]);

	config->dir = argv[argc-2];
	config->command = argv[argc-1];

	if (argc == 3)
		return;

	// Parsing parameters
	for (int i = 1; i < argc-2; i++) 
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'l':
				config->loop = true;
				break;
			default:
				printf("unrecognized parameter '%s'\n", argv[i]);
				usage(argv[0]);

			}
		} else {
			printf("unrecognized '%s'\n", argv[i]);
			usage(argv[0]);
		}

}

int
run_trigger(char *dir, char *command, int filter)
{
	struct kevent event;
	struct kevent tevent;
	int kq, fd, ret;

	fd = open(dir, O_RDONLY);
	if (fd == -1)
		err(1, "Failed to open '%s'", dir);

	kq = kqueue();
	if (kq == -1)
		err(1, "kqueue() failed");

	EV_SET(&event, fd, filter, EV_ADD | EV_CLEAR, NOTE_WRITE, 0, NULL);
	ret = kevent(kq, &event, 1, NULL, 0, NULL);
	if (ret == -1)
		err(1, "kevent register");
	if (event.flags & EV_ERROR)
		errx(1, "Event error: %s", strerror(event.data));

	/* Sleep until something happens. */
	ret = kevent(kq, NULL, 0, &tevent, 1, NULL);
	if (ret == -1)
		err(1, "kevent wait");
	ret = system(command);
	return (ret);
}
