#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: photobase.py,v 1.17 2011-07-14 07:34:50 grahn Exp $
# $Name:  $
#
# Copyright (c) 2001, 2004, 2005, 2008, 2011, 2013 Jörgen Grahn
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

def _extract_keys(s):
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


def extract_keys(s):
    try:
        return _extract_keys(s)
    except:
        raise ParseError(s)


class ParseError(Exception):
    "parse error"


class Photo:
    """Just a collection of attributes describing the photo:
    'filename'    - path-less file name
    'datetime'    - creation date and time as a string (e.g. '2002-10-26 14:58')
                    or None
    'description' - textual description
    'keys'        - a list of key strings
    'category'
    'url'

    'description' and 'keys' are generated when requested, since the key parsing
    is expensive.
    """

    datetimere = re.compile(r'^(\d{4})-(\d{2})-(\d{2})'
                            '\s+([012]\d:[0-5]\d)$')

    def __init__(self, ss, prev=None):
        if len(ss) < 1:
            raise ParseError(`ss`)
        self.filename = ss[0]
        datetime = ss[1]
        if self.datetimere.match(datetime):
            self.datetime = datetime
            s = ' '.join(ss[2:])
        else:
            self.datetime = None
            s = ' '.join(ss[1:])
        self.category = None
        self.url = None
        if prev and s=='ibid':
            self._description = prev._description
        else:
            self._description = s

    def __getattr__(self, name):
        if name in ('description', 'keys'):
            self.description, self.keys = extract_keys(self._description)
            return self.__dict__[name]
        else:
            raise AttributeError(name)

class Photobase:
    """A collection of photos. Public attributes:
    'photos' - the list of Photo instances
    'files'  - the mapping file name -> Photo instance
    'keys'   - the mapping keyword string -> list of Photo instances

    'keys' is generated when first requested.
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

    def __getattr__(self, name):
        if name=='keys':
            self.keys = self._collect_keys()
            return self.keys
        else:
            raise AttributeError(name)

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
        self.photos.append(Photo(ss, prev))
        ph = self.photos[-1]
        self.files[ph.filename] = ph
        return ph

    def _collect_keys(self):
        for ph in self.photos:
            for k in ph.keys:
                try:
                    self.keys[k].append(ph)
                except KeyError:
                    self.keys[k] = [ph]

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
                          '-name', '.xvpics', '-prune',
                          '-or',
                          '-name', 'thumb', '-prune',
                          '-or', '-print'))
        n = len(basepath)
        baseurl = baseurl.rstrip('/')
        for s in f:
            if not s.startswith(basepath):
                continue
            relpath = s[n:].lstrip('/').rstrip()
            category, filename = os.path.split(relpath)
            if goodnames and not goodnames.has_key(filename):
                continue
            url = '%s/%s' % (baseurl,relpath)
            self.files[filename] = (category, url)
            try:
                self.categories[category].append(filename)
            except KeyError:
                self.categories[category] = [filename]
        os.wait()

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
        self.photos = [ x for x in self.photos if x.url ]

        keys = {}
        for ph in self.photos:
            for k in ph.keys:
                try:
                    keys[k].append(ph)
                except KeyError:
                    keys[k] = [ph]
        self.keys = keys


_percentSplit = re.compile(r'(%[A-Fa-f0-9]{2})').split

def _utf8_to_latin1(s):
    return unicode(s, 'utf-8').encode('latin-1')

def _latin1_to_utf8(s):
    return unicode(s, 'latin-1').encode('utf-8')

def uri_decode(s):
    """Decode an Request-URI (potentially containing %xx crap,
    in turn hiding UTF-8 crap) into a Latin-1 string.
    """
    acc = _percentSplit(s)
    for n in range(1, len(acc), 2):
        acc[n] = chr(int(acc[n][1:], 16))
    return _utf8_to_latin1(''.join(acc))

def uri_encode(s):
    """The rough inverse to uri_decode().
    """
    def _may_percent(ch):
        """Percent-encode octets outside the unreserved set
        """
        if ch in 'abcdefghijklmnopqrstuvwxyz':
            return ch
        if ch in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
            return ch
        if ch in '-._~0123456789':
            return ch
        return '%%%02X' % ord(ch)
    s = _latin1_to_utf8(s)
    return ''.join([_may_percent(ch) for ch in s])
    acc = _percentSplit(s)
    for n in range(1, len(acc), 2):
        acc[n] = chr(int(acc[n][1:], 16))
    return _utf8_to_latin1(''.join(acc))


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
