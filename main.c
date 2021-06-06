#include "ktrigger.h"

int
main(int argc, char **argv)
{
	struct config_s config;
	init_config(&config);
	parse_cmd(&config, argc, argv);
	int ret;
	do {
		ret = run_trigger(config.dir, config.command, config.filter);
	} while (config.loop);
	return (ret);
}
