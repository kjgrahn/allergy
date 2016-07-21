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
all: allergyd
all: parse
all: tests

.PHONY: install
install: allergyd
install: allergyd.1
install: allergy.5
	install -m755 allergyd $(INSTALLBASE)/bin/
	install -m644 allergyd.1 $(INSTALLBASE)/man/man1/
	install -m644 allergy.5 $(INSTALLBASE)/man/man5/

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

liballergy.a: version.o
liballergy.a: log.o
liballergy.a: server.o
liballergy.a: content.o
liballergy.a: root.o
liballergy.a: times.o
liballergy.a: session.o
liballergy.a: sessionhistory.o
liballergy.a: error.o
liballergy.a: textread.o
liballergy.a: requestqueue.o
liballergy.a: request.o
liballergy.a: date.o
liballergy.a: dateparse.o
liballergy.a: names.o
liballergy.a: filter.o
liballergy.a: backlog.o
liballergy.a: chunk.o
liballergy.a: deflate.o
liballergy.a: response.o
liballergy.a: join.o
liballergy.a: glob.o
liballergy.a: allergy/thumbnail.o
liballergy.a: entity/string.o
liballergy.a: entity/file.o
liballergy.a: allergy/keys.o
liballergy.a: allergy/photo.o
liballergy.a: allergy/files...o
	$(AR) -r $@ $^

backlog.o: CXXFLAGS+=-Wno-old-style-cast
deflate.o: CXXFLAGS+=-Wno-old-style-cast
httpd.o: CXXFLAGS+=-Wno-old-style-cast

allergyd: httpd.o liballergy.a
	$(CXX) $(CXXFLAGS) -o $@ httpd.o -L. -lallergy -lrt -lz

magic: magic.o liballergy.a
	$(CXX) $(CXXFLAGS) -o $@ magic.o -L. -lallergy -lmagic

parse: allergy/parse.o liballergy.a
	$(CXX) $(CXXFLAGS) -o $@ allergy/parse.o -L. -lallergy

#libtest.a: test/test_response.o
libtest.a: test/test_request.o
libtest.a: test/test_filter.o
libtest.a: test/test_blob.o
libtest.a: test/test_chunk.o
libtest.a: test/test_log.o
libtest.a: test/pipe.o
libtest.a: test/test_deflate.o
libtest.a: test/test_cache.o
libtest.a: test/test_date.o
libtest.a: test/test_lineparse.o
libtest.a: test/test_join.o
libtest.a: test/test_glob.o
libtest.a: test/test_regex.o
libtest.a: allergy/test/test_keys.o
libtest.a: allergy/test/test_photo.o
libtest.a: allergy/test/test_files.o
	$(AR) -r $@ $^

test/%.o: CPPFLAGS+=-I.
allergy/test/%.o: CPPFLAGS+=-I.
allergy/test/test_files.o: CPPFLAGS+=-Iallergy

test.cc: libtest.a
	orchis -o$@ $^

tests: test.o liballergy.a libtest.a
	$(CXX) $(CXXFLAGS) -o $@ test.o -L. -ltest -lallergy -lz

%.1.ps : %.1
	groff -man $< >$@

%.5.ps : %.5
	groff -man $< >$@

.PHONY: tags TAGS
tags: TAGS
TAGS:
	etags *.cc *.h

depend:
	makedepend -- $(CXXFLAGS) $(CPPFLAGS) -- -Y -I. {,test/}*.cc allergy/{,test/}*.cc

.PHONY: clean
clean:
	$(RM) allergyd magic parse
	$(RM) {,entity/,test/}*.o
	$(RM) allergy/{,test/}*.o
	$(RM) *.ps
	$(RM) lib*.a
	$(RM) test.cc tests
	$(RM) Makefile.bak TAGS
	$(RM) -r dep/

love:
	@echo "not war?"

$(shell mkdir -p dep/{entity/,allergy/,}{,test})
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/entity/%.d: ;
dep/entity/test/%.d: ;
dep/test/%.d: ;
dep/allergy/%.d: ;
dep/allergy/test/%.d: ;
-include dep/*.d
-include dep/entity/*.d
-include dep/test/*.d
-include dep/allergy/*.d
-include dep/allergy/test/*.d
