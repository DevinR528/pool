

#include <stdio.h>
#include <stdlib.h>

#include <defines/argv0.h>

#include <enums/error.h>

#include <debug.h>

#include "smalloc.h"

int smalloc(void** out, size_t size)
{
	int error = 0;
	void* ptr = malloc(size);
	
	if (!ptr)
		fprintf(stderr, "%s: malloc(%lu): %m\n", argv0, size),
		error = e_out_of_memory;
	else
		*out = ptr;
	
	return error;
}

