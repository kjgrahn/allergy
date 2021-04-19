/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_QUOTE_H
#define ALLERGY_QUOTE_H

#include <string>
#include <iosfwd>

/**
 * For escaping special characters special to HTML 4 text, when
 * putting on an ostream:
 * < and &. (HTML 4.01, section 5.3.2).
 */
struct quote {
    explicit quote(const std::string& s) : s{s} {}
    const std::string& s;
};

std::ostream& operator<< (std::ostream& os, const quote& val);

namespace attr {

    /**
     * For escaping special characters special to HTML 4 attributes,
     * when putting on an ostream:
     * ' and ".
     */
    struct quote {
	explicit quote(const std::string& s) : s{s} {}
	const std::string& s;
    };

    std::ostream& operator<< (std::ostream& os, const quote& val);
}

#endif
