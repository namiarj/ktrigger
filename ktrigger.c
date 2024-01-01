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

static u_int
get_event(const char *arg)
{
	return NOTE_WRITE;
}

static short
get_filter(const char *arg)
{
	const short filter[] = {EVFILT_READ, EVFILT_WRITE, EVFILT_EMPTY, EVFILT_AIO,
		EVFILT_VNODE, EVFILT_PROC, EVFILT_PROCDESC, EVFILT_SIGNAL, EVFILT_TIMER};

	const char *filter_arg[] = {"read", "write", "empty", "aio", "vnode",
		"proc", "procdesc", "signal", "timer"};

	for (int i = 0; i < (sizeof filter / sizeof filter[0]); i++)
		if (strcmp(arg, filter_arg[i]) == 0)
			return (filter[i]);
	usage();
	return (1);
}

static void
usage()
{
	const char *usage_msg = "usage: ktrigger -f filter [-e fflag] [-l count] [-d dir] [-c command]\n";
	fputs(usage_msg, stderr);
	exit(1);
}

void
parse_cmd(struct ktrigger *kt, int argc, char **argv) 
{
	kt->cmd = kt->dir = NULL;
	kt->fflags = kt->loop = kt->filter = 0;

	int ch;
	while ((ch = getopt(argc, argv, "f:e:l:d:t:c:")) != -1) {
		switch (ch) {
		case 'c':
			kt->cmd = optarg;
			break;
		case 'd':
			kt->dir = optarg;
			break;
		case 'e':
			kt->fflags |= get_event(optarg);
			break;
		case 'f':
			kt->filter = get_filter(optarg);
			break;
		case 'l':
			sscanf(optarg, "%d", &kt->loop);
			break;
		case 't':
		default:
			usage();
		}
	}

	if (!kt->filter)
		usage();

	if (kt->filter == EVFILT_VNODE && !kt->fflags)
		usage();

}

int
run_trigger(struct ktrigger *kt)
{
	struct kevent event;
	struct kevent tevent;
	int kq, fd, ret;

	fd = open(kt->dir, O_RDONLY);
	if (fd == -1)
		err(1, "Failed to open '%s'", kt->dir);

	kq = kqueue();
	if (kq == -1)
		err(1, "kqueue() failed");

	EV_SET(&event, fd, kt->filter, EV_ADD | EV_CLEAR, kt->fflags, 0, NULL);

	ret = kevent(kq, &event, 1, NULL, 0, NULL);
	if (ret == -1)
		err(1, "kevent register");
	if (event.flags & EV_ERROR)
		errx(1, "Event error: %s", strerror(event.data));

	/* Sleep until something happens. */
	ret = kevent(kq, NULL, 0, &tevent, 1, NULL);
	if (ret == -1)
		err(1, "kevent wait");

	if (kt->cmd) {
		ret = system(kt->cmd);
		return (ret);
	}

	return (0);

}
