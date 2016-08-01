/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "index.h"

#include "files...h"

using namespace allergy;

namespace {

    bool empty(const std::string& s)
    {
	return s.empty();
    }

    bool comment(const std::string& s)
    {
	return !empty(s) && s[0]=='#';
    }

    bool isspace(char ch)
    {
	return std::isspace(static_cast<unsigned char>(ch));
    }

    /**
     * Trim trailing whitespace from 's'.
     */
    void trimr(std::string& s)
    {
	while (s.size()) {
	    if (!isspace(s.back())) break;
	    s.pop_back();
	}
    }

    allergy::Entry parse(const std::vector<std::string>& v,
			 const std::vector<allergy::Entry>& prev)
    {
	const auto end = v.end();
	auto p = v.begin();
	auto filename = *p++;
	if(p==end) return {filename, p, p, {}, {}};

	const auto& ibid = prev.empty()? "" : prev.back().text;

	const allergy::Timestamp ts(*p++);
	if (!ts.valid()) {
	    return {filename, p-1, end, {}, ibid};
	}

	return {filename, p, end, ts, ibid};
    }
}

Index::Index(Files& in)
{
    std::string s;
    std::vector<std::string> v;
    auto has_entry = [&v] () { return !v.empty(); };

    auto emit = [&] () {
		    Entry e = parse(v, entries);
		    entries.push_back(e);
		    v.clear();
		};

    while(in.getline(s)) {
	if(in.first() && has_entry()) {
	    emit();
	}

	trimr(s);
	if(comment(s)) continue;

	if(empty(s) && has_entry()) emit();

	if(!empty(s)) v.emplace_back(s);
    }

    if(has_entry()) emit();
}
