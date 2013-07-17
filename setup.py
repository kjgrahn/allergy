#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: setup.py,v 1.4 2008-10-20 21:11:22 grahn Exp $
#
from distutils.core import setup

def cvs_says(dollarname='$Name:  $'):
    import re
    m = re.match(r'\$'r'Name:\s+(.+?)-(\d+(-\d+)*)\D', dollarname)
    if not m: return ('allergy', 'unknown')
    return m.group(1), m.group(2).replace('-', '.')

_, version = cvs_says()

setup(name = 'allergy',
      version = version,
      description = 'thumbnail and HTML picture gallery creator',

      author = 'Jörgen Grahn',
      author_email = 'grahn+src@snipabacken.se',
      license = 'GPL',
      url = 'http://snipabacken.se/~grahn/comp/#allergy',

      py_modules = ['photobase'],
      # A real Makefile will install the programs ...
      )
