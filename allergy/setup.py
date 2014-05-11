#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: setup.py,v 1.4 2008-10-20 21:11:22 grahn Exp $
#
from distutils.core import setup

setup(name = 'allergy',
      version = '5.10',
      description = 'thumbnail and HTML picture gallery creator',

      author = 'Jörgen Grahn',
      author_email = 'grahn+src@snipabacken.se',
      license = 'GPL',
      url = 'http://snipabacken.se/~grahn/comp/#allergy',

      py_modules = ['photobase'],
      # A real Makefile will install the programs ...
      )
