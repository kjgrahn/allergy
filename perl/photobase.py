#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: photobase.py,v 1.11 2008-08-07 21:17:00 grahn Exp $
# $Name:  $
#
# Copyright (c) 2001, 2004, 2005, 2008 Jörgen Grahn
# All rights reserved.
#
"""parse text files describing digital photos
"""
import re
import sys
import os
import time

_bracketsub = re.compile(r'[\[\]]').sub
_squeezewhitesub = re.compile(r'\s{2,}').sub
_angledre = re.compile(r'\{(.+?)\}')

def extract_keys(s):
    """From string 's', extract [[bracketed] keys, even nested
    ones]. Also extract and remove keys in {angle brackets}.

    Returns the unbracketed text, and a list of unique keys.
    """
    keys = []
    try:
        while 1:
            keys.append(_angledre.search(s).group(1))
            s = _angledre.sub('', s, 1)
    except AttributeError:
        pass
    stack = []
    i = 0
    for ch in s:
        if ch=='[':
            stack.append(i)
        elif ch==']':
            k = stack.pop()
            keys.append(s[k+1: i])
        i += 1
    s = _squeezewhitesub(' ', _bracketsub('', s))
    keys = [ _squeezewhitesub(' ', _bracketsub('', x)) for x in keys ]
    keymap = {}
    for k in keys:
        keymap[k] = None
    return s, keymap.keys()


class ParseError(Exception):
    "parse error"


class Photo:
    """Just a collection of attributes describing the photo:
    'filename'    - path-less file name
    'datetime'    - creation date and time as a string (e.g. '2002-10-26 14:58')
    'description' - textual description
    'keys'        - a list of key strings
    'category'
    'url'
    """

    datetimere = re.compile(r'^(\d{4})-(\d{2})-(\d{2})'
                            '\s+([012]\d:[0-5]\d)$')

    def __init__(self, ss, prev=None):
        if len(ss) < 2:
            raise ParseError(`ss`)
        self.filename = ss[0]
        self.datetime = ss[1]
        self.category = None
        self.url = None
        if not self.datetimere.match(self.datetime):
            raise ParseError(`ss`)
        s = ' '.join(ss[2:])
        if prev and s=='ibid':
            self.description = prev.description
            self.keys = prev.keys
        else:
            self.description, self.keys = extract_keys(s)

class Photobase:
    """A collection of photos. Public attributes:
    'photos' - the list of Photo instances
    'files'  - the mapping file name -> Photo instance
    'keys'   - the mapping keyword string -> list of Photo instances
    """

    emptyre = re.compile(r'^\s*$')
    commentre = re.compile(r'^#')

    def __init__(self, paths):
        self.photos = []
        self.files = {}
        self.keys = {}
        for p in paths:
            self._append(open(p, 'r'))
        if not paths:
            self._append(sys.stdin)

    def _append(self, f):
        acc = []
        prev = None
        for s in f:
            if self.commentre.match(s):
                continue
            if self.emptyre.match(s):
                if acc:
                    prev = self._appendp(acc, prev)
                    acc = []
                continue
            acc.append(s.rstrip())
        if acc:
            prev = self._appendp(acc, prev)

    def _appendp(self, ss, prev):
        ph = Photo(ss, prev)
        self.photos.append(ph)
        self.files[ph.filename] = ph
        for k in ph.keys:
            try:
                self.keys[k].append(ph)
            except KeyError:
                self.keys[k] = [ph]
        return ph

    def has(self, file):
        "Do we have info on the image file 'file'?"
        return self.files.has_key(file)

    def text(self, file):
        "The descriptive text for the image file 'file'."
        text = self.files[file].description
        return text.replace('"', "'")

    def time(self, file):
        """The creation date and time for the image file 'file' as a
        string 'yyyy-mm-dd hh:mm'.
        """
        return self.files[file].datetime


class Names:
    """Images as found in the file system, and what they look like
    as URLs.

    'files'      - a mapping from filename to (category, url)
    'categories' - a mapping from category to filename list

    XXX overworked
    """
    def __init__(self, basepath, baseurl,
                 goodnames = None):
        self.files = {}
        self.categories = {}
        _, f = os.popen2(('find', basepath,
                          '-type', 'f', '-print',
                          '-or',
                          '-name', '.xvpics', '-prune'))
        n = len(basepath)
        baseurl = baseurl.rstrip('/')
        for s in f:
            if not s.startswith(basepath):
                continue
            relpath = s[n:].lstrip('/').rstrip()
            category, filename = os.path.split(relpath)
            if goodnames and not goodnames.has_key(filename):
                continue
            url = '%s/%s' % (baseurl, relpath)
            self.files[filename] = (category, url)
            try:
                self.categories[category].append(filename)
            except KeyError:
                self.categories[category] = [filename]

class Superbase:
    """A tarted up Photobase.
    """
    def __init__(self, descriptions,
                 basepath, baseurl):
        self._descriptions = descriptions
        self._basepath = basepath
        self._baseurl = baseurl
        self._time = 0
        self.refresh()

    def refresh(self):
        now = time.time()
        if now < self._time + 5:
            return
        for p in self._descriptions:
            if os.path.getmtime(p) > self._time:
                break
        else:
            return
        pb = Photobase(self._descriptions)
        self._time = time.time()
        self._photobase = pb
        self.photos = pb.photos
        self.files = pb.files
        self.keys = pb.keys
        names = Names(self._basepath, self._baseurl)
        for p in self.photos:
            try:
                p.category, p.url = names.files[p.filename]
            except KeyError:
                pass

if __name__ == "__main__":
    import getopt

    pb = Photobase([])

    if 0:
        for p in pb.photos:
            print p.description
    else:
        kk = pb.keys.keys()
        kk.sort()
        for k in kk:
            print k
            for ph in pb.keys[k]:
                print '  ', ph.description
