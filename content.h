/* -*- c++ -*-
 *
 * Copyright (c) 2017, 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_CONTENT_H_
#define OUTN_CONTENT_H_

#include <string>
#include <regex>

class Request;
class Response;

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

    Response* response_of(const Request& request) const;

private:
    const Patterns re;
    const std::string host;
    const int root;

    Response* frontpage() const;
    Response* by_date() const;
    Response* year(unsigned yyyy) const;
    Response* month(unsigned yyyy, unsigned mm) const;
    Response* redirect(const std::string& s) const;
    Response* photo(const std::string& s) const;
    Response* thumb(const std::string& s) const;
    Response* keywords() const;
    Response* keyword(const std::string& s) const;
    Response* robots() const;
    Response* css() const;
    Response* favicon() const;
};

#endif
