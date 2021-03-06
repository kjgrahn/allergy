/*
 * Copyright (c) 2013 J?rgen Grahn
 * All rights reserved.
 *
 */
#include "pipe.h"
#include <blob.h>

#include <orchis.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


Pipe::Pipe()
    : rfd(-1),
      wfd(-1)
{
    int fd[2];
    (void)pipe2(fd, O_NONBLOCK);
    rfd = fd[0];
    wfd = fd[1];
}


Pipe::~Pipe()
{
    close(rfd);
    close(wfd);
}


/**
 * Predicate: it's possible to read exactly 's' from the pipe.
 * It may contain more stuff.
 */
void Pipe::assert_read(const std::string& s)
{
    std::string t = s;
    ssize_t n = read(rfd, &t[0], t.size());
    orchis::assert_eq(n, t.size());
    orchis::assert_eq(s, t);
}


void Pipe::assert_read(const Blob& s)
{
    assert_read(std::string(s.begin(), s.end()));
}


size_t Pipe::drain(size_t len)
{
    std::string t(len, ' ');
    ssize_t n = read(rfd, &t[0], t.size());
    orchis::assert_(n!=-1 || errno==EAGAIN);
    if(n==-1) n = 0;
    return n;
}


/**
 * Like assert_read(s), but doesn't check the contents.
 */
void Pipe::assert_drain(size_t len)
{
    size_t n = drain(len);
    orchis::assert_eq(n, len);
}


/**
 * The pipe is empty.
 */
void Pipe::assert_empty()
{
    char buf[1];
    ssize_t n = read(rfd, buf, sizeof buf);
    orchis::assert_eq(n, -1);
    orchis::assert_eq(errno, EAGAIN);
}
