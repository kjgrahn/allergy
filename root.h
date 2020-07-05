/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_ROOT_H_
#define OUTN_ROOT_H_

#include <string>

/**
 * A root directory, from which you can openat(2) files with relative
 * paths.
 *
 * We don't use the openat(2) style for security reasons but for
 * convenience; it maps well to HTTP.
 */
class Root {
public:
    explicit Root(const std::string& path);
    ~Root();

    int open(const std::string& relpath) const;

private:
    const int dirfd;
};

#endif
