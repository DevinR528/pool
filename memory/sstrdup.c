
#include <stdio.h>
#include <string.h>

#include <enums/error.h>

#include <defines/argv0.h>

#include <debug.h>

#include "sstrdup.h"

int sstrdup(char** out, const char* in)
{
	int error = 0;
	ENTER;
	
	char* dup = strdup(in);
	
	if (!dup)
		fprintf(stderr, "%s: strdup(): %m\n", argv0),
		error = e_out_of_memory;
	else
		*out = dup;
	
	EXIT;
	return error;
}

