# $Id: Makefile,v 1.7 2010-07-28 12:39:37 grahn Exp $
#
# Makefile
#
# Copyright (c) 2010 J�rgen Grahn
# All rights reserved.

SHELL=/bin/sh
INSTALLBASE=/usr/local
CXXFLAGS=-Wall -Wextra -pedantic -std=c++98 -g -O3
CPPFLAGS=-I..

.PHONY: all
all: libgresabladet.a
all: gresabladet

.PHONY: install
install: gresabladet
install: gresabladet.1
	install -m755 gresabladet $(INSTALLBASE)/bin/
	install -m644 gresabladet.1 $(INSTALLBASE)/man/man1/

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests

libgresabladet.a: version.o
libgresabladet.a: client.o
libgresabladet.a: response.o
	$(AR) -r $@ $^

gresabladet: gresabladet.o libgresabladet.a
	$(CXX) -o $@ gresabladet.o -L. -lgresabladet -L../sockutil -lsocket

libtest.a: 
	$(AR) -r $@ $^

test.cc: libtest.a
	testicle -o$@ $^

tests: test.o libgresabladet.a libtest.a
	$(CXX) -o $@ test.o -L. -ltest -lgresabladet

test/%.o : test/%.cc
	$(CXX) -I. $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%.1.ps : %.1
	groff -man $< >$@

.PHONY: tags
tags: TAGS
TAGS:
	etags *.cc *.h

depend:
	makedepend -- $(CXXFLAGS) $(CPPFLAGS) -- -Y -I. *.cc test/*.cc

.PHONY: clean
clean:
	$(RM) gresabladet
	$(RM) *.o
	$(RM) *.ps
	$(RM) libgresabladet.a
	$(RM) libtest.a
	$(RM) test/*.o
	$(RM) test.cc tests
	$(RM) Makefile.bak TAGS

love:
	@echo "not war?"

# DO NOT DELETE

client.o: client.h ../sockutil/textread.h response.h
gresabladet.o: version.h client.h ../sockutil/textread.h
response.o: response.h
version.o: version.h
