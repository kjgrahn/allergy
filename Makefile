# $Id: Makefile,v 1.4 2008-08-02 21:45:17 grahn Exp $
#
# Makefile
#
# Copyright (c) 2001 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.
# 

SHELL = /bin/sh

INSTALLBASE = /usr/local

all:

install:
	python ./setup.py install --prefix $(INSTALLBASE) --force
	install -m755 perl/allergy $(INSTALLBASE)/bin/
	install -m755 perl/allergy_index $(INSTALLBASE)/bin/
	install -m644 doc/allergy.1 $(INSTALLBASE)/man/man1/
	install -m644 doc/allergy_index.1 $(INSTALLBASE)/man/man1/

clean:
	python ./setup.py clean
	$(RM) -r build dist

love:
	@echo "not war?"
