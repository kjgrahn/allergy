/* -*- c++ -*-
 *
 * Copyright (c) 2017, 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_CONTENT_H_
#define OUTN_CONTENT_H_

#include "root.h"
#include "allergy/photo.h"

#include <string>
#include <regex>

class Request;
class Response;
class timespec;

struct Patterns {
    Patterns();

    std::regex frontpage;
    std::regex by_date;
    std::regex year;
    std::regex month;

    std::regex photo;
    std::regex thumb;

    std::regex key;
    std::regex keywords;
    std::regex keyword;

    std::regex css;
    std::regex robots;
    std::regex favicon;
};

/**
 * The server content, defined by all responses to all possible
 * requests.  Or, defined by the overall virtual file system, with
 * static HTML, dynamic content and images.
 *
 * Note that it's shared by all sessions, and could leak information
 * between users (if we had any to leak).
 */
class Content {
public:
    Content(const std::string& host, const std::string& path);
    ~Content();
    Content(const Content&) = delete;
    Content& operator= (const Content&) = delete;

    Response* response_of(const Request& request, const timespec& ts) const;

private:
    const Patterns re;
    const std::string host;
    const Root lib;
    const Root thumb;
    const Root root;

    Response* frontpage(const timespec& t) const;
    Response* by_date(const timespec& t) const;
    Response* year(const timespec& t, unsigned yyyy) const;
    Response* month(const timespec& t, unsigned yyyy, unsigned mm) const;
    Response* redirect(const timespec& t, const std::string& s) const;
    Response* photo(const timespec& t, const allergy::Photo& p) const;
    Response* thumbnail(const timespec& t, const allergy::Photo& p) const;
    Response* keywords(const timespec& t) const;
    Response* keyword(const timespec& t, const std::string& s) const;
    Response* robots(const timespec& t) const;
    Response* css(const timespec& t) const;
    Response* favicon(const timespec& t) const;
};

#endif
