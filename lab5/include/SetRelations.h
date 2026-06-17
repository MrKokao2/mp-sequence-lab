#pragma once

#include "IntSet.h"

#include <cstddef>

namespace setrel {

struct Relations {
    bool is_union = false;
    bool is_intersection = false;
    bool is_diff_ab = false;
    bool is_diff_ba = false;
};

int DeriveThreadCount(std::size_t sizeA, std::size_t sizeB, std::size_t sizeC);

Relations CheckRelationsSerial(const IntSet& a, const IntSet& b, const IntSet& c);

Relations CheckRelationsParallel(const IntSet& a, const IntSet& b,
                                 const IntSet& c, int threads = 0);

}  // namespace setrel
