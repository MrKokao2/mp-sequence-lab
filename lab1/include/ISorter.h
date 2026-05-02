#pragma once

#include "ArraySequence.h"
#include <string>

template<typename T>
class ISorter {
public:
    virtual ~ISorter() = default;

    virtual ArraySequence<T> Sort(const ArraySequence<T>& sequence) = 0;
    virtual std::string GetName() const = 0;
};