#include "ktrigger.h"

int
main(int argc, char **argv)
{
	int ret;
	struct ktrigger kt;
	parse_cmd(&kt, argc, argv);
loop:
	ret = run_trigger(&kt);

	if (kt.loop < -1 || (kt.loop > 0 && --kt.loop))
		goto loop;

	return (ret);
}
