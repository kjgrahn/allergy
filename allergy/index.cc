/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "index.h"

#include "files...h"

#include <iostream>
#include <algorithm>

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

	const auto& ibid = prev.empty()? nullptr : &prev.back();

	const allergy::Timestamp ts(*p++);
	if (!ts.valid()) {
	    return {filename, p-1, end, {}, ibid};
	}

	return {filename, p, end, ts, ibid};
    }

    void invert(std::ostream& err,
		std::map<Photo, unsigned>& names,
		const std::vector<Entry>& ee)
    {
	unsigned n = 0;
	for (const Entry& e: ee) {
	    auto res = names.emplace(e.filename, n);
	    if (!res.second) {
		err << "warning: duplicate name '" << e.filename << "'\n";
	    }
	    n++;
	}
    }

    void invert(std::map<Key, std::vector<unsigned>>& keys,
		const std::vector<Entry>& ee)
    {
	unsigned n = 0;
	for (const Entry& e: ee) {
	    for (const Key& k: e.keywords) {
		keys[k].push_back(n);
	    }
	    n++;
	}
    }
}

Index::Index(std::ostream& err, Files& in)
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

    invert(err, by.name, entries);
    invert(by.key, entries);

    if (!entries.empty()) {
	Year min = entries.front().timestamp.year;
	Year max = min;
	for (const Entry& e: entries) {
	    max = std::max(max, e.timestamp.year);
	    min = std::min(min, e.timestamp.year);
	}
	years.begin = min;
	years.end = max;
	years.end++;
    }
}

namespace {

    void sort(std::vector<Entry>& ee)
    {
	auto earlier = [] (const Entry& a, const Entry& b) {
			   return a.timestamp < b.timestamp;
		       };
	std::stable_sort(begin(ee), end(ee), earlier);
    }

    template <class Pred>
    std::vector<Entry> get(const Index& ix, Pred p)
    {
	std::vector<Entry> v;
	std::copy_if(ix.begin(), ix.end(), std::back_inserter(v), p);
	sort(v);
	return v;
    }
}

bool Index::has(const Photo& p) const
{
    return by.name.count(p);
}

std::vector<Entry> Index::all() const
{
    std::vector<Entry> v{begin(), end()};
    sort(v);
    return v;
}

std::vector<Entry> Index::in(const Year& key) const
{
    auto p = [&key] (const Entry& e) { return e.timestamp.year == key; };
    return get(*this, p);
}

std::vector<Entry> Index::in(const Month& key) const
{
    auto p = [&key] (const Entry& e) { return e.timestamp.month == key; };
    return get(*this, p);
}

std::vector<Entry> Index::on(const Day& key) const
{
    auto p = [&key] (const Entry& e) { return e.timestamp.date == key; };
    return get(*this, p);
}

std::vector<Entry> Index::key(const Key& key) const
{
    auto it = by.key.find(key);
    if (it==by.key.end()) return {};

    std::vector<Entry> v;
    for (unsigned n: it->second) {
	v.push_back(entries[n]);
    }
    sort(v);
    return v;
}
