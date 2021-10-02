/* -*- c++ -*-
 *
 * Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef ALLERGYD_HOSTNAMES_H_
#define ALLERGYD_HOSTNAMES_H_

#include <vector>
#include <string>

class Blob;

/**
 * The allowed Host headers [14.23].
 */
class Hostnames {
public:
    void add(const char* name);
    bool empty() const { return val.empty(); }

    bool match(const Blob& s) const;

private:
    std::vector<std::string> val;
};

#endif
