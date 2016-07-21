/*
 * Copyright (c) 2016 Jörgen Grahn
 * All rights reserved.
 *
 */
#include <iostream>
#include <getopt.h>

#include "files...h"

namespace {

    bool empty(const std::string& s)
    {
	return s.empty();
    }

    bool comment(const std::string& s)
    {
	return !empty(s) && s[0]=='#';
    }

    void emit(std::ostream& os,
	      const Files&,
	      std::vector<std::string>& v)
    {
	for(const std::string& s: v) {
	    os << s << '\n';
	}
	os << '\n';
	v.clear();
    }

    void parse(std::ostream& os, Files& in)
    {
	std::string s;
	std::vector<std::string> v;
	auto has_entry = [&v] () { return !v.empty(); };

	while(in.getline(s)) {
	    if(in.first() && has_entry()) {
		emit(os, in, v);
	    }

	    if(comment(s)) continue;

	    if(empty(s) && has_entry()) emit(os, in, v);

	    if(!empty(s)) v.emplace_back(s);
	}

	if(has_entry()) emit(os, in, v);
    }
}

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
    parse(std::cout, files);

    return 0;
}
