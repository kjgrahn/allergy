# $Id: Makefile,v 1.5 2009-03-18 21:42:51 grahn Exp $
#
# Makefile
#
# Copyright (c) 2001, 2009 Jörgen Grahn <grahn+src@snipabacken.se>
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
	install -m644 allergy-mode.el $(INSTALLBASE)/share/emacs/site-lisp/

clean:
	python ./setup.py clean
	$(RM) -r build dist

love:
	@echo "not war?"
