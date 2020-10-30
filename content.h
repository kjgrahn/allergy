/* -*- c++ -*-
 *
 * Copyright (c) 2017, 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_CONTENT_H_
#define OUTN_CONTENT_H_

#include "root.h"
#include "allergy/index.h"
#include "allergy/calendar.h"
#include "allergy/keyword.h"

#include <iosfwd>
#include <string>

class Request;
class Response;
class timespec;

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
    Content(std::ostream& err,
	    const std::string& host,
	    const allergy::Index& index,
	    const std::string& path);
    ~Content();
    Content(const Content&) = delete;
    Content& operator= (const Content&) = delete;

    bool valid() const;

    Response* response_of(const Request& request, const timespec& ts) const;

private:
    const std::string host;
    const allergy::Index& index;
    const Root lib;
    const Root thumb;
    const Root root;

    Response* frontpage(const timespec& t) const;
    Response* by_date(const timespec& t) const;
    Response* calendar(const timespec& t, const allergy::Year&) const;
    Response* calendar(const timespec& t, const allergy::Month&) const;
    Response* redirect(const timespec& t, const std::string& s) const;
    Response* photo(const timespec& t, const allergy::Photo& p) const;
    Response* thumbnail(const timespec& t, const allergy::Photo& p) const;
    Response* keywords(const timespec& t) const;
    Response* keyword(const timespec& t, const allergy::Key&) const;
    Response* robots(const timespec& t) const;
    Response* css(const timespec& t) const;
    Response* favicon(const timespec& t) const;
};

#endif
