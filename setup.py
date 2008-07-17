#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: setup.py,v 1.2 2008-07-17 21:44:59 grahn Exp $
#
from distutils.core import setup

def cvs_says(dollarname='$Name:  $'):
    import re
    m = re.match(r'\$'r'Name:\s+(.+?)-(\d+(-\d+)*)\D', dollarname)
    if not m: return ('allergy', 'unknown')
    return m.group(1), m.group(2).replace('-', '.')

name, version = cvs_says()

setup(name = name,
      version = version,
      description = 'thumbnail and HTML picture gallery creator',

      author = 'Jörgen Grahn',
      author_email = 'grahn+src@snipabacken.se',
      license = 'GPL',
      url = 'http://snipabacken.se/~grahn/comp/',

      package_dir = {'': 'perl'},
      py_modules = ['photobase'],
      scripts = ['perl/allergy'])
