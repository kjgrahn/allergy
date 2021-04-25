/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "photo.h"
#include "../root.h"
#include "../strtou.h"

#include <cctype>
#include <sstream>
#include <algorithm>


using allergy::Serial;
using allergy::Photo;

namespace {

    unsigned short narrow(unsigned n)
    {
	return static_cast<unsigned short>(n);
    }
}

Serial::Serial(const char* a, const char* b)
    : digits {narrow(b-a)}
{
    n = narrow(::strtou(a, b, 65000));
    if (a!=b) digits = 0;
}

Serial::Serial(const std::string& s)
    : Serial {s.data(), s.data() + s.size()}
{}

std::ostream& Serial::put(std::ostream& os) const
{
    char buf[10];
    std::snprintf(buf, sizeof buf, "%0*hu", digits, n);
    return os << buf;
}

namespace {

    /**
     * Parse a yymmdd date in [a, b).
     */
    template <class It>
    allergy::Day shortdate(It a, const It b)
    {
	unsigned n = strtou(a, b, 1e6);
	if (a != b) return {};
	unsigned dd = n % 100; n /= 100;
	unsigned mm = n % 100; n /= 100;
	unsigned yyyy = 2000 + n;
	return {yyyy, mm, dd};
    }
}

/**
 * Construct from a photo filename, which is one of
 *   yyyy-mm-dd_serial.jpg
 *   yymmdd_serial.jpg
 */
Photo::Photo(const char* a, const char* const b)
{
    auto c = std::find(a, b, '_');
    if (c==b) return;

    if (c-a == 10) {
	day = {a, c};
    }
    else if (c-a == 6) {
	day = shortdate(a, c);
    }
    else {
	return;
    }

    a = c + 1;
    c = std::find(a, b, '.');
    if (std::string(c, b) != ".jpg") { day = {}; return; }

    serial = {a, c};
}

Photo::Photo(const std::string& s)
    : Photo {s.data(), s.data() + s.size()}
{}

bool Photo::operator== (const Photo& other) const
{
    return day==other.day && serial==other.serial;
}

bool Photo::operator< (const Photo& other) const
{
    if (day==other.day) return serial < other.serial;
    return day < other.day;
}

std::ostream& Photo::put(std::ostream& os) const
{
    return os << day << '_' << serial << ".jpg";
}

/**
 * The directory (relative to some root) where this
 * photo's file lives.  Year and quarter, e.g. 2021.2.
 */
std::string Photo::dir() const
{
    if (!valid()) return {};
    return day.quarter();
}

/**
 * The relative URI of this photo (the actual image).
 */
std::string Photo::url() const
{
    std::ostringstream oss;
    oss << '/' << *this;
    return oss.str();
}

/**
 * The relative URI of this photo's page.
 */
std::string Photo::pageurl() const
{
    std::ostringstream oss;
    oss << '/' << day << '/' << serial;
    return oss.str();
}

/**
 * The relative URI of this photo's thumbnail.
 */
std::string Photo::thumburl() const
{
    std::ostringstream oss;
    oss << "/thumb/" << *this;
    return oss.str();
}

