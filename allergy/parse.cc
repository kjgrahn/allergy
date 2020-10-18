/*
 * Copyright (c) 2016 Jörgen Grahn
 * All rights reserved.
 *
 */
#include <iostream>
#include <getopt.h>

#include "files...h"
#include "index.h"

int main(int argc, char ** argv)
{
    const std::string prog = argv[0];
    const std::string usage = "usage: "
	+ prog +
	" file ...";
    const char optstring[] = "";
    const struct option long_options[] = {
	{"help",    	   0, 0, 'h'},
	{0, 0, 0, 0}
    };

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring, &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'h':
	    std::cout << usage << '\n';
	    return 0;
	case ':':
	case '?':
	    std::cerr << usage << '\n';
	    return 1;
	    break;
	default:
	    break;
	}
    }

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    Files files(argv+optind, argv+argc);
    const allergy::Index ix(std::cerr, files);

    const char* sep = "";
    for (const allergy::Entry& e: ix.all()) {
	std::cout << sep
		  << e.filename << '\n'
		  << e.timestamp << '\n'
		  << e.text << '\n';
	sep = "\n";
    }

    return 0;
}
