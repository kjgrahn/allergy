/* -*- c++ -*-
 *
 * Copyright (c) 2017, 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_CONTENT_H_
#define OUTN_CONTENT_H_

#include "root.h"
#include "date.h"

#include "allergy/index.h"
#include "allergy/calendar.h"
#include "allergy/keyword.h"

#include <iosfwd>
#include <string>

class Request;
class Hostnames;
class Response;
class timespec;

/**
 * The server content, defined by all responses to all possible
 * requests.  Or, defined by the overall virtual file system, with
 * static HTML, dynamic content and images.
 *
 * Note that it's shared by all sessions, and could leak information
 * between users (if we had any to leak, but the fact that it's all
 * const shows that we don't).
 */
class Content {
public:
    Content(std::ostream& err,
	    const Hostnames& hosts,
	    const allergy::Index& index,
	    const std::string& path);
    ~Content();
    Content(const Content&) = delete;
    Content& operator= (const Content&) = delete;

    bool valid() const;

    Response* response_of(const Request& request) const;

    const Hostnames& hosts;
    const allergy::Index& index;

    const DateConv dateconv;
    std::string format(const timespec& t) const { return dateconv.format(t.tv_sec); }
    const timespec epoch;

    const Root lib;
    const Root thumb;
    const Root root;

private:
    Response* frontpage(const Request& req) const;
    Response* by_date(const Request& req) const;
    Response* calendar(const Request& req, const allergy::Year&) const;
    Response* calendar(const Request& req, const allergy::Month&) const;
    Response* calendar(const Request& req, const allergy::Day&) const;
    Response* redirect(const Request& req, const std::string& s) const;
    Response* photo(const Request& req, const allergy::Photo& p) const;
    Response* thumbnail(const Request& req, const allergy::Photo& p) const;
    Response* photopage(const Request& req, const allergy::Day&,
			const allergy::Serial&) const;
    Response* keywords(const Request& req) const;
    Response* keyword(const Request& req, const allergy::Key&) const;
    Response* robots(const Request& req) const;
    Response* css(const Request& req) const;
    Response* favicon(const Request& req) const;
};

#endif
