#
# Makefile
#
# Copyright (c) 2010--2014 Jörgen Grahn
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local
CXXFLAGS=-Wall -Wextra -pedantic -std=c++11 -g -Os -Wold-style-cast
CPPFLAGS=

.PHONY: all
all: outnumbered
all: tests

.PHONY: install
install: outnumbered
install: outnumbered.1
	install -m755 outnumbered $(INSTALLBASE)/bin/
	install -m644 outnumbered.1 $(INSTALLBASE)/man/man1/

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

liboutnumbered.a: version.o
liboutnumbered.a: log.o
liboutnumbered.a: server.o
liboutnumbered.a: times.o
liboutnumbered.a: session.o
liboutnumbered.a: sessionhistory.o
liboutnumbered.a: textread.o
liboutnumbered.a: requestqueue.o
liboutnumbered.a: request.o
liboutnumbered.a: date.o
liboutnumbered.a: names.o
liboutnumbered.a: filter.o
liboutnumbered.a: deflate.o
liboutnumbered.a: response.o
liboutnumbered.a: input.o
	$(AR) -r $@ $^

filter.o: CXXFLAGS+=-Wno-old-style-cast
deflate.o: CXXFLAGS+=-Wno-old-style-cast
httpd.o: CXXFLAGS+=-Wno-old-style-cast

liballergy.a: allergy/keys.o
	$(AR) -r $@ $^

outnumbered: httpd.o liboutnumbered.a
	$(CXX) $(CXXFLAGS) -o $@ httpd.o -L. -loutnumbered -lrt -lz

magic: magic.o liboutnumbered.a
	$(CXX) $(CXXFLAGS) -o $@ magic.o -L. -loutnumbered -lmagic

#libtest.a: test/test_response.o
#libtest.a: test/test_responsebuf.o
libtest.a: test/test_request.o
libtest.a: test/test_filter.o
libtest.a: test/test_log.o
libtest.a: test/pipe.o
libtest.a: test/test_deflate.o
libtest.a: test/test_cache.o
libtest.a: test/test_date.o
libtest.a: test/test_lineparse.o
libtest.a: allergy/test/test_keys.o
	$(AR) -r $@ $^

test/%.o: CPPFLAGS+=-I.
allergy/test/%.o: CPPFLAGS+=-I.

test.cc: libtest.a
	orchis -o$@ $^

tests: test.o liboutnumbered.a liballergy.a libtest.a
	$(CXX) $(CXXFLAGS) -o $@ test.o -L. -ltest -loutnumbered -lallergy -lz

%.1.ps : %.1
	groff -man $< >$@

%.5.ps : %.5
	groff -man $< >$@

.PHONY: tags
tags: TAGS
TAGS:
	etags *.cc *.h

depend:
	makedepend -- $(CXXFLAGS) $(CPPFLAGS) -- -Y -I. {,test/}*.cc allergy/{,test/}*.cc

.PHONY: clean
clean:
	$(RM) outnumbered magic
	$(RM) {,test/}*.o
	$(RM) allergy/{,test/}*.o
	$(RM) *.ps
	$(RM) lib*.a
	$(RM) test.cc tests
	$(RM) Makefile.bak TAGS
	$(RM) -r dep/

love:
	@echo "not war?"

$(shell mkdir -p dep/{allergy/,}test)
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/test/%.d: ;
dep/allergy/%.d: ;
dep/allergy/test/%.d: ;
-include dep/*.d
-include dep/test/*.d
-include dep/allergy/*.d
-include dep/allergy/test/*.d
