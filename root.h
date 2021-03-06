/* -*- c++ -*-
 *
 * Copyright (c) 2020 J?rgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_ROOT_H_
#define OUTN_ROOT_H_

#include <string>

/**
 * A root directory, from which you can openat(2) files with relative
 * paths and perform other relative file system operations.
 *
 * We don't use the openat(2) style for security reasons but for
 * convenience and clarity; it maps well to HTTP.
 */
class Root {
public:
    explicit Root(const std::string& path);
    ~Root();
    Root(const Root&) = delete;
    Root& operator= (const Root&) = delete;

    bool valid() const { return dirfd != -1; }

    int open(const std::string& relpath) const;
    bool unlink(const std::string& name) const;
    bool mkdir(const std::string& name) const;
    bool chdir() const;

private:
    const int dirfd;
};

inline int open(const Root& r, const std::string& relpath)
{
    return r.open(relpath);
}

#endif
