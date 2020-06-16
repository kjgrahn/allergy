/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_FILE_H_
#define OUTN_FILE_H_

/**
 * The name of a file, as relative path an the fd specifying the
 * directory it's relative to, openat(2)-style.
 *
 * We don't use the openat(2) style for security reasons but for
 * convenience; it maps well to HTTP.
 */
class File {
public:
    template <class T>
    File(int dir, T path)
	: dir{dir},
	  path{path}
    {}

    int open() const;

private:
    const int dir;
    const std::string path;
};

#endif
