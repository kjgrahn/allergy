/* -*- c++ -*-
 *
 * Copyright (c) 2017 Jörgen Grahn
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
    explicit Content(const std::string& host, const std::string& path);
    Response* response_of(const Request& request) const;

private:
    const Patterns re;
    const std::string host;
};

#endif
