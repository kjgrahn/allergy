/* -*- c++ -*-
 *
 * Copyright (c) 2012, 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_REQUESTQUEUE_H_
#define GB_REQUESTQUEUE_H_

#include "request.h"

#include <queue>
#include <string>


/**
 * The (partial) HTTP requests of a session. Grows line by line, and
 * at any time contains zero or more complete requests, and zero or
 * one partial request.
 */
class RequestQueue {
public:
    RequestQueue() = default;
    RequestQueue(const RequestQueue&) = delete;
    RequestQueue& operator= (const RequestQueue&) = delete;

    void add(const timespec& t, const char* a, const char* b);
    bool complete() const;
    bool broken() const;

    const Request& front() const;
    void pop();

private:
    std::queue<Request> val;
};

#endif
