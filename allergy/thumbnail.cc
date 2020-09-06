/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "thumbnail.h"

#include <sstream>
#include <array>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

using allergy::Photo;


namespace {

    int wait_for(pid_t pid)
    {
	int status;
	waitpid(pid, &status, 0);
	return status;
    }

    char* next(char*& p)
    {
	while(*p) p++;
	p++;
	return p;
    }

    constexpr char nil = '\0';

    /**
     * Constructing the argv for execvp(2).
     */
    template <size_t N>
    class Argv {
    public:
	explicit Argv(const std::ostringstream& oss)
	    : buf{oss.str()}
	{
	    char* p = &buf.front();
	    char** q = val.data();
	    *q++ = p;
	    for (size_t i=1; i<N; i++) {
		*q++ = next(p);
	    }
	    *q++ = nullptr;
	}
	Argv(const Argv&) = delete;
	Argv& operator= (const Argv&) = delete;

	std::array<char*, N+1> val;
    private:
	std::string buf;
    };

    [[noreturn]]
    void djpeg(int wfd, const Root& src, const Photo& photo)
    {
	std::ostringstream oss;
	oss << "djpeg" << nil
	    << "-scale" << nil
	    << "1/8" << nil
	    << photo.path();
	Argv<4> argv(oss);
	dup2(wfd, 1); close(wfd);
	close(0); open("/dev/null", O_RDONLY);
	if (src.chdir()) {
	    execvp("djpeg", argv.val.data());
	}
	std::exit(1);
    }

    [[noreturn]]
    void cjpeg(int rfd, const Root& dst, const Photo& photo)
    {
	std::ostringstream oss;
	oss << "cjpeg" << nil
	    << "-qual" << nil
	    << "10" << nil
	    << "-optimize" << nil
	    << "-progressive" << nil
	    << "-outfile" << nil
	    << photo.path();
	Argv<7> argv(oss);
	dup2(rfd, 0); close(rfd);
	close(1); open("/dev/null", O_WRONLY);
	if (dst.chdir()) {
	    execvp("cjpeg", argv.val.data());
	}
	std::exit(1);
    }
}

/**
 * More or less
 *   (cd $src && djpeg -scale 1/8 $photo) |
 *   (cd $dst && cjpeg -qual 10 -opt -prog -outfile $photo)
 *
 * This doesn't thumbnail to a specific size, but you still get a file
 * that's a lot smaller than the original (0.1--0.7% in my first tests).
 *
 * Ignores Exif information; the image must be valid as a plain JPEG,
 * specifically not require interpretation of the Exif 'Orientation'
 * flag.
 *
 */
bool allergy::thumbnail(const Root& src,
			const Root& dst,
			const Photo& photo)
{
    /* Pipeline. The unused stdin and stdout are set to /dev/null;
     * stderr is unchanged. So is any other file descriptor, so e.g.
     * cjpeg may get a gazillion TCP sockets. Unclear what to do about
     * that; live with it?
     *
     * Sloppy error handling, except when acquiring resources.
     *
     * Avoid deadlocks by making the reader exit when the writer does,
     * due to end of file. For that, the parent needs to carefully
     * close its copy of the pipe, too.
     */

    int pipe[2];
    const int& rfd = pipe[0];
    const int& wfd = pipe[1];
    int err = pipe2(pipe, 0);
    if (err) return false;

    const pid_t cpid = fork();
    if (!cpid) {
	close(wfd);
	cjpeg(rfd, dst, photo);
    }
    close(rfd);
    if (cpid==-1) {
	close(wfd);
	return false;
    }

    const pid_t dpid = fork();
    if (!dpid) {
	djpeg(wfd, src, photo);
    }
    close(wfd);
    if (dpid==-1) {
	wait_for(cpid);
	return false;
    }

    const int cstatus = wait_for(cpid);
    const int dstatus = wait_for(dpid);

    if(!WIFEXITED(dstatus) || !WIFEXITED(cstatus)) return false;
    if(WEXITSTATUS(dstatus) || WEXITSTATUS(cstatus)) return false;
    return true;
}
