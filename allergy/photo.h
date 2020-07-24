/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_PHOTO_H
#define ALLERGY_PHOTO_H

#include <string>

namespace allergy {

    /**
     * A photo, from the perspective of finding it in the file system
     * based on the name in an URL.
     *
     * A photo is named 'yyyy-mm-dd_serial.jpg', where 'serial' is
     * also a non-empty sequence of digits.
     */
    class Photo {
    public:
	explicit Photo(const std::string& s);
	Photo(const char* a, const char* b);

	bool valid() const { return quarter; }

    private:
	std::string val;
	unsigned short quarter;
    };
}

#endif
