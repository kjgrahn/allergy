# $Id: Makefile,v 1.3 2008-07-17 22:12:10 grahn Exp $
#
# Makefile
#
# Copyright (c) 2001 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.
# 

SHELL = /bin/sh

INSTALLBASE = /usr/local

all:

install: perl/allergy doc/allergy.1
	python ./setup.py install --prefix $(INSTALLBASE) --force
	install -m755 perl/allergy $(INSTALLBASE)/bin/
	install -m644 doc/allergy.1 $(INSTALLBASE)/man/man1/

clean:
	python ./setup.py clean
	$(RM) -r build dist

love:
	@echo "not war?"
