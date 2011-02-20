/* -*- c++ -*-
 * $Id: dbfile.h,v 1.1 2011-02-20 07:52:41 grahn Exp $
 *
 * Copyright (c) 2011 J�rgen Grahn
 * All rights reserved.
 *
 */
#ifndef GB_DBFILE_H_
#define GB_DBFILE_H_

/**
 */
class DbFile {
public:
    DbFile(const std::string& file, int mode);

private:
    DbFile();
    DbFile(const DbFile&);
    DbFile& operator= (const DbFile&);

};

#endif
