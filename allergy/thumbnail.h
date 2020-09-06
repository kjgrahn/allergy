/* -*- c++ -*-
 *
 * Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#ifndef OUTN_THUMBNAIL_H
#define OUTN_THUMBNAIL_H

#include "../root.h"
#include "photo.h"

namespace allergy {

    bool thumbnail(const Root& src,
		   const Root& dst,
		   const Photo& photo);
}

#endif
