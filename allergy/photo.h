/* -*- c++ -*-
 *
 * Copyright (c) 2020, 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_PHOTO_H
#define ALLERGY_PHOTO_H

#include <string>
#include <iostream>

class Root;

namespace allergy {

    /**
     * The serial number of a Photo. A non-empty sequence of digits,
     * but it could have been anything else which distinguishes photos
     * from the same date and is reflected in the file name.
     */
    class Serial : public std::string {
    public:
	Serial(const char* a, const char* b) : std::string {a, b} {}
	explicit Serial(const char* s) : std::string {s} {}
    };

    /**
     * A photo, from the perspective of finding it in the file system
     * based on the name in an URL.
     *
     * A photo is named 'yyyy-mm-dd_serial.jpg'. To avoid having thousands
     * of photos in one directory, we split them across directories
     * named "yyyy.q". Thus, the URL namespace becomes flatter than the
     * filesystem.
     *
     * Photos names 'yymmdd_serial.jpg' are also supported, for legacy
     * reasons.
     *
     * Thumbnails work exactly like Photos, but relative to another
     * Root.
     */
    class Photo {
    public:
	explicit Photo(const std::string& s);
	Photo(const char* a, const char* b);

	bool operator== (const Photo& other) const { return val==other.val; }
	bool operator<  (const Photo& other) const { return val<other.val; }
	std::ostream& put(std::ostream& os) const;

	bool valid() const { return quarter; }
	explicit operator bool() const { return valid(); }
	std::string dir() const;
	std::string path() const;

	std::string url() const;
	std::string thumburl() const;

	int open(const Root& root) const;

    private:
	std::string val;
	unsigned short quarter;
    };

    int open(const Root& r, const Photo& p);

    inline
    std::ostream& operator<< (std::ostream& os, const Photo& val)
    {
	return val.put(os);
    }
}

#endif
