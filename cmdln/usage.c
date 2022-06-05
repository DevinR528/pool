
#include <stdio.h>

#include <debug.h>

#include <defines/argv0.h>

#include "usage.h"

void usage()
{
	printf("usage: %s [-vh] [-j=jobs] <input-file.calc>\n", argv0);
}


