
#include <stdio.h>
#include <stdlib.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <debug.h>

#include "srealloc.h"

int srealloc(void** inout, size_t newsize)
{
	int error = 0;
	ENTER;
	
	void* new = realloc(*inout, newsize);
	
	if (!new)
		fprintf(stderr, "%s: realloc(%lu): %m\n", argv0, newsize),
		error = e_out_of_memory;
	else
		*inout = new;
	
	EXIT;
	return error;
}

