/* Copyright (c) 2012, 2013, 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "response.h"


bool response::Headers::tick(int fd)
{
    return text.tick(fd, filter);
}

bool response::Headers::done() const
{
    return text.done();
}

bool response::File::tick(int fd)
{
    bool blocked = response::tick(fd, backlog, headers, body);
    done = body.done();
    return blocked;
}

bool response::Image::tick(int fd)
{
    bool blocked = response::tick(fd, backlog, headers, body);
    done = body.done();
    return blocked;
}
