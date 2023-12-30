#include "ktrigger.h"

int
main(int argc, char **argv)
{
	int ret;
	struct config config;
	parse_cmd(&config, argc, argv);
loop:
	ret = run_trigger(config.dir, config.command, config.filter);

	if (config.loop == -1)
		goto loop;

	if (config.loop > 0 && --config.loop)
		goto loop;

	return (ret);
}
