
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <debug.h>

#include <defines/argv0.h>
#include <defines/DEFAULT_NUMBER_OF_THREADS.h>

#include <enums/error.h>

#include <memory/smalloc.h>

#include "struct.h"
#include "usage.h"
#include "process.h"

int cmdln_process(struct cmdln** out, int argc, char* argv[])
{
	int error = 0;
	ENTER;
	
	const char* input_path = NULL;
	unsigned number_of_threads = DEFAULT_NUMBER_OF_THREADS;
	bool verbose = false;
	
	int opt, option_index;
	const struct option long_options[] = {
		{"input",  required_argument, 0, 'i'},
		{"jobs",   required_argument, 0, 'j'},
		{"verbose",      no_argument, 0, 'v'},
		{"help",         no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};
	
	while (!error && (opt = getopt_long(argc, argv,
		"i:" "j:" "v" "h", long_options, &option_index)) >= 0)
	{
		switch (opt)
		{
			case 'i':
				input_path = optarg;
				break;
			
			case 'j':
			{
				char* m;
				unsigned long int value;
				
				dpvs(optarg);
				
				if (errno = 0, value = strtoul(optarg, &m, 10), errno || *m)
					fprintf(stderr, "%s: invalid parameter for -j: '%s'!\n", argv0, optarg),
					error = e_bad_cmdline_args;
				else if (value > 100)
					fprintf(stderr, "%s: parameter for -j way too high!\n", argv0),
					error = e_bad_cmdline_args;
				else
					number_of_threads = value;
				
				break;
			}
			
			case 'v':
				verbose = true;
				break;
			
			case 'h':
				error = e_bad_cmdline_args;
				break;
			
			default:
				error = e_bad_cmdline_args;
				break;
		}
	}
	
	if (!error && !input_path)
	{
		fprintf(stderr, "%s: missing input file!\n", argv0);
		error = e_bad_cmdline_args;
	}
	
	if (error == e_bad_cmdline_args)
		usage();
	
	struct cmdln* flags = NULL;
	
	if (!error)
		error = smalloc((void**) &flags, sizeof(*flags));
	
	if (!error)
	{
		flags->input_path = input_path;
		flags->number_of_threads = number_of_threads;
		
		flags->verbose = verbose;
		
		dpvs(flags->input_path);
		dpv(flags->number_of_threads);
		dpvb(flags->verbose);
		
		*out = flags;
	}
	
	EXIT;
	return error;
}
















