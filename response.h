/* -*- c++ -*-
 *
 * Copyright (c) 2012, 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_RESPONSE_H_
#define GB_RESPONSE_H_

/**
 * A complete, specific HTTP response. We have already decided /how/
 * we want to respond (using which status code and so on) and all that
 * is left is to respond.
 *
 * If it hadn't been for our poll-driven/nonblocking architecture,
 * this could have been a straight function call: write headers;
 * read from file and write to socket until we're done.
 *
 * As it is, we have to be prepared for any socket write to be partial
 * or EWOULDBLOCK, in which case we have to be prepared to remember
 * our state and resume later.
 *
 * To summarize, write(fd) means try one socket write operation, and
 * the result is one of:
 * - ok to write more immediately
 * - wait for the socket to turn writable, then continue writing
 * - done; the response has been written completely.
 *
 */
class Response {
public:
    Response() {}
    Response(const Response&) = delete;
    Response& operator= (const Response&) = delete;
    virtual ~Response() {}

    virtual bool write(int fd) = 0;
    virtual bool done() const = 0;

private:
};


class Request;
Response* response_of(const Request& request);

#endif
