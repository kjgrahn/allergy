/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "chunk.h"

#include <algorithm>

using namespace allergy::page;

container::Chunk::Chunk(unsigned size, size_t seq_size)
    : size {size},
      n {std::max(1u, unsigned((seq_size + size - 1) / size))},
      i {0}
{}
