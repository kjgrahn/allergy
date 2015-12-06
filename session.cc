/* $Id: session.cc,v 1.12 2011-07-03 19:40:17 grahn Exp $
 *
 * Copyright (c) 2010, 2011, 2013 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "session.h"
#include "log.h"

#include <iostream>
#include <sstream>
#include <cassert>

#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>


namespace {

    std::ostream& operator<< (std::ostream& os, const sockaddr_storage& sa)
    {
	char hbuf[300];
	char sbuf[30];
	int rc = getnameinfo(reinterpret_cast<const struct sockaddr*>(&sa),
			     sizeof sa,
			     hbuf, sizeof hbuf,
			     sbuf, sizeof sbuf,
			     NI_NOFQDN
			     | NI_NUMERICHOST
			     | NI_NUMERICSERV);
	if(rc) {
	    return os << "unknown(" << strerror(errno) << ')';
	}
	return os << hbuf << ':' << sbuf;
    }

    std::string getnameinfo(const sockaddr_storage& sa)
    {
	std::ostringstream oss;
	oss << sa;
	return oss.str();
    }
}


Session::Session(const sockaddr_storage& peer,
		 const timespec& t)
    : peer(peer),
      history(t),
      reader("\r\n"),
      response(0)
{
    Info(Syslog::log) << *this << " connected";
}


Session::~Session()
{
    Info(Syslog::log) << *this << " closed. " << history;
    delete response;
}


/**
 * The socket is readable and we claimed to be interested in reading;
 * advance the state machine by doing so.
 *
 */
Session::State Session::read(int fd, const timespec& t)
{
    reader.feed(fd);

    char* a;
    char* b;
    while(reader.read(a, b)) {
	req_queue.add(a, b);
    }

    /* At this point these are the interesting states
     * (reader.eof(); req_queue.complete(); response):
     *  /__________________/                  /
     * E Q Resp -----------------------------'
     * -----
     * � y n  WRITING; new Response
     * � � y  WRITING
     * n n n  READING
     * y n n  DIE
     */

    if(response) return WRITING;
    if(req_queue.broken()) return DIE;
    if (req_queue.complete()) {
	pop_req(t);
	return WRITING;
    }

    return reader.eof() ? DIE : READING;
}


/**
 * The socket is writeable and we claimed to be interested in writing;
 * advance the state machine by doing so.
 */
Session::State Session::write(int fd, const timespec& t)
{
    while(response) {
	history.wrote(t);
	if(!response->write(fd)) {
	    return WRITING;
	}

	if(response->done()) {
	    history.ended(*response, t);
	    delete response;
	    response = 0;

	    /* XXX how about broken()? */
	    if (req_queue.complete()) {
		pop_req(t);
	    }
	}
    }

    return reader.eof() ? DIE : READING;
}


/**
 * Ugly little helper.
 */
void Session::pop_req(const timespec& t)
{
    assert(req_queue.complete());
    assert(!response);

    const Request req = req_queue.front();
    Info(Syslog::log) << *this << ' ' << req.method << ' ' << req.request_uri();
    std::cout << req << '\n';
    req_queue.pop();
    response = response_of(req);
    history.began(*response, t);
}


/**
 * It's 't' o'clock; is the session is such a bad shape that the best
 * thing to do is kill it unilaterally?
 *
 * XXX Should take overall load into account, not just the history of
 * this particular session.
 */
bool Session::reconsider(const timespec& t)
{
    return history.idle(10, t) || history.wedged(10, t);
}


/**
 * The session is identified simply by host:port.
 */
std::ostream& Session::put(std::ostream& os) const
{
    return os << peer;
}
