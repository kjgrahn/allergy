# $Id: Makefile,v 1.2 2008-07-17 22:08:36 grahn Exp $
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
	install -m755 perl/allergy $(INSTALLBASE)/bin/
	install -m644 doc/allergy.1 $(INSTALLBASE)/man/man1/

clean:

love:
	@echo "not war?"
