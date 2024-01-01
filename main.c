#include "ktrigger.h"

int
main(int argc, char **argv)
{
	int ret;
	struct ktrigger kt;
	parse_cmd(&kt, argc, argv);
	do {
		ret = run_trigger(&kt);
	} while (kt.loop < 0 || (kt.loop > 0 && --kt.loop));
	return (ret);
}
