/* -*- c++ -*-
 *
 * Copyright (c) 2020, 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGY_PHOTO_H
#define ALLERGY_PHOTO_H

#include "calendar.h"

#include <string>
#include <iostream>

class Root;

namespace allergy {

    /**
     * The serial number of a Photo. A non-empty sequence of digits,
     * but it could have been anything else which distinguishes photos
     * from the same date and is reflected in the file name.
     */
    class Serial {
    public:
	Serial() = default;
	Serial(const char* a, const char* b);
	explicit Serial(const std::string& s);

	bool operator== (const Serial& other) const { return n==other.n; }
	bool operator<  (const Serial& other) const { return n < other.n; }
	std::ostream& put(std::ostream& os) const;
	explicit operator bool() const { return digits; }

	unsigned short digits = 0;
	unsigned short n = 0;
    };

    inline
    std::ostream& operator<< (std::ostream& os, const Serial& val)
    {
	return val.put(os);
    }

    /**
     * A photo, from the perspective of finding it in the file system
     * based on the name in an URL.
     *
     * It's not a file name: it's a Day and a Serial number. It can be
     * /derived/ from a file name however, if you restrict the file
     * names (which we do). So '2021-04-25_0101.jpg', '2021-04-25_0101.png'
     * and even '210425_0101.jpg' are the same photo.
     *
     * The URI follows the Photo rather than the file name. So a HTTP
     * GET translates to a Photo, and then you have to make an Index
     * lookup for the file name, or the thumbnail file name.
     */
    class Photo {
    public:
	Photo() = default;
	Photo(Day day, Serial serial) : day{day}, serial{serial} {}
	explicit Photo(const std::string& s);
	Photo(const char* a, const char* b);

	bool operator== (const Photo& other) const;
	bool operator<  (const Photo& other) const;
	std::ostream& put(std::ostream& os) const;

	bool valid() const { return day && serial; }
	explicit operator bool() const { return valid(); }

	std::string dir() const;
	std::string url() const;
	std::string pageurl() const;
	std::string thumburl() const;

    private:
	Day day;
	Serial serial;
    };

    inline
    std::ostream& operator<< (std::ostream& os, const Photo& val)
    {
	return val.put(os);
    }
}

#endif
