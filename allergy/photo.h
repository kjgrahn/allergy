/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_PHOTO_H
#define ALLERGY_PHOTO_H

#include <string>

class Root;

namespace allergy {

    /**
     * A photo, from the perspective of finding it in the file system
     * based on the name in an URL.
     *
     * A photo is named 'yyyy-mm-dd_serial.jpg', where 'serial' is
     * also a non-empty sequence of digits. To avoid having thousands
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

	bool valid() const { return quarter; }
	std::string dir() const;
	std::string path() const;

	int open(const Root& root) const;

    private:
	std::string val;
	unsigned short quarter;
    };

    int open(const Root& r, const Photo& p);
}

#endif
