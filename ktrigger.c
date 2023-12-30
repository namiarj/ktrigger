#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>

#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ktrigger.h"

static void
usage()
{
	char *usage_msg = "usage: ktrigger -f filter [-l count] [-d dir] [-c command]\n";
	fputs(usage_msg, stderr);
	exit(1);
}

void
parse_cmd(struct config *config, int argc, char **argv) 
{
	config->command = config->dir = NULL;
	config->filter = 0;
	config->loop = 0;

	int ch;
	while ((ch = getopt(argc, argv, "f:l:d:c:")) != -1) {
		switch (ch) {
		case 'c':
			config->command = optarg;
			break;
		case 'd':
			config->dir = optarg;
			break;
		case 'f':
			config->filter = EVFILT_VNODE;
			break;
		case 'l':
			sscanf(optarg, "%d", &config->loop);
			break;
		default:
			usage();
		}
	}

	if (!config->filter)
		usage();
		
	if (!config->dir)
		usage();
}

int
run_trigger(char *dir, char *command, short filter)
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

	if (command) {
		ret = system(command);
		return (ret);
	}

	return (0);

}
