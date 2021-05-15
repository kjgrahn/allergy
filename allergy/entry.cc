/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "entry.h"
#include "../root.h"

#include <iostream>

using allergy::Entry;

template <class It>
Entry::Entry(const std::string& filename,
	     It begin,
	     It end,
	     const Timestamp& timestamp,
	     const Entry* const prev)
    : photo {filename},
      filename {filename},
      timestamp {timestamp}
{
    const Keys keys {util::join("\n", begin, end)};
    text = keys.str();
    for(const std::string& key : keys) {
	keywords.emplace(key);
    }

    if (text=="ibid" && prev) {
	text = prev->text;
	keywords = prev->keywords;
    }
}

template
Entry::Entry(const std::string& filename,
	     std::vector<std::string>::const_iterator,
	     std::vector<std::string>::const_iterator,
	     const Timestamp& timestamp,
	     const Entry* const prev);

std::ostream& operator<< (std::ostream& os, const Entry& val)
{
    os << val.filename << '\n';
    if(val.timestamp.valid()) {
	os << val.timestamp << '\n';
    }
    return os << val.text;
}

std::string allergy::path(const Entry& e)
{
    return e.photo.dir() + '/' + e.filename;
}

int open(const Root& r, const Entry& e)
{
    return open(r, path(e));
}
