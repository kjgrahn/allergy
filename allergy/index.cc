/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "index.h"

#include "files...h"
#include "../status.h"
#include "../lower_bound.h"

#include <iostream>
#include <sstream>
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

    /**
     * Weed out duplicate entries (entries for the same Photo) in a
     * sorted list, and log while doing it. Duplicates would have been
     * mostly harmless, but would have messed with prev/next(photo).
     */
    void weed(std::ostream& err, std::vector<Entry>& ee)
    {
	auto eq = [] (const Entry& a, const Entry& b) {
	    return a.photo == b.photo;
	};

	/* Have to do two passes, because the trailing elements left
	 * by std::unique at the end are more likely to be garbage
	 * Entries than the weeded ones.
	 */
	auto a = begin(ee);
	const auto b = end(ee);
	while (1) {
	    a = std::adjacent_find(a, b, eq);
	    if (a==b) break;
	    a++;
	    err << "warning: ignoring duplicate entry '" << a->photo << "'\n";
	}

	a = std::unique(begin(ee), b, eq);
	ee.erase(a, b);
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
	if (e) {
	    entries.push_back(e);
	}
	else {
	    err << in.position() << ": ignoring bad entry '" << e.filename << "'\n";
	}
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

    std::stable_sort(begin(entries), end(entries));
    weed(err, entries);

    invert(by.key, entries);

    if (!entries.empty()) {
	years = {
	    entries.front().timestamp.day.year(),
	    entries.back().timestamp.day.year()
	};
	years.end++;
    }
}

namespace {

    auto search(const std::vector<allergy::Entry>& ee, const Photo& p)
    {
	auto it = ::lower_bound(begin(ee), end(ee),
				[p] (const Entry& e) { return e.photo < p; });
	if (it==end(ee)) return it;
	if (it->photo == p) return it;
	return end(ee);
    }
}

/**
 * True iff the index contains 'p'.
 */
bool Index::has(const Photo& p) const
{
    auto it = search(entries, p);
    return it != end(entries);
}

/**
 * Get the entry for 'p', or throw 404.
 */
const Entry& Index::get(const Photo& p) const
{
    auto it = search(entries, p);
    if (it != end(entries)) return *it;
    throw Status<404> {};
}

/**
 * All photos, in order.
 */
Index::Range Index::all() const
{
    return {begin(entries),
	    end(entries)};
}

namespace {

    Year  year(const Timestamp& ts)  { return ts.day.year(); }
    Month month(const Timestamp& ts) { return ts.day.month(); }
    Day   day(const Timestamp& ts)   { return ts.day; }
}

/**
 * All photos for a certain year.
 */
Index::Range Index::in(const Year& key) const
{
    auto a = ::lower_bound(begin(entries), end(entries),
			   [key] (const Entry& e) { return year(e.timestamp) < key; });
    auto b = ::lower_bound(a, end(entries),
			   [key] (const Entry& e) { return year(e.timestamp) <= key; });
    return {a, b};
}

/**
 * All photos for a certain month.
 */
Index::Range Index::in(const Month& key) const
{
    auto a = ::lower_bound(begin(entries), end(entries),
			   [key] (const Entry& e) { return month(e.timestamp) < key; });
    auto b = ::lower_bound(a, end(entries),
			   [key] (const Entry& e) { return month(e.timestamp) <= key; });
    return {a, b};
}

/**
 * All photos for a certain day.
 */
Index::Range Index::on(const Day& key) const
{
    auto a = ::lower_bound(begin(entries), end(entries),
			   [key] (const Entry& e) { return day(e.timestamp) < key; });
    auto b = ::lower_bound(a, end(entries),
			   [key] (const Entry& e) { return day(e.timestamp) <= key; });
    return {a, b};
}

/**
 * The last Day before 'day' with photos, or the invalid Day if there
 * are no earlier photos. Note that there need not be any photos from
 * 'day' for this to work.
 */
Day Index::prev(const Day& day) const
{
    auto a = ::lower_bound(begin(entries), end(entries),
			   [day] (const Entry& e) { return e.timestamp.day < day; });
    if (a==begin(entries)) return {};
    return std::prev(a)->timestamp.day;
}

/**
 * The first Day after 'day' with photos.
 */
Day Index::next(const Day& day) const
{
    auto a = ::lower_bound(begin(entries), end(entries),
			   [day] (const Entry& e) { return e.timestamp.day <= day; });
    if (a==end(entries)) return {};
    return a->timestamp.day;
}

/**
 * The photo before 'p', or Photo {}.
 */
Photo Index::prev(const Photo& p) const
{
    auto it = search(entries, p);
    if (it==end(entries)) return {};
    if (it==begin(entries)) return {};
    it--;
    return it->photo;
}

/**
 * The photo after 'p', or Photo {}.
 */
Photo Index::next(const Photo& p) const
{
    auto it = search(entries, p);
    if (it==end(entries)) return {};
    it++;
    if (it==end(entries)) return {};
    return it->photo;
}

std::vector<Entry> Index::key(const Key& key) const
{
    auto it = by.key.find(key);
    if (it==by.key.end()) return {};

    std::vector<Entry> v;
    for (unsigned n: it->second) {
	v.push_back(entries[n]);
    }
    std::stable_sort(begin(v), end(v));
    return v;
}
