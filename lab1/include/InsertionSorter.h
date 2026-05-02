#pragma once

#include "ISorter.h"

template<typename T>
class InsertionSorter : public ISorter<T> {
public:
    ArraySequence<T> Sort(const ArraySequence<T>& sequence) override {
        ArraySequence<T> result = sequence;

        for (int i = 1; i < result.getLength(); ++i) {
            T key = result.Get(i);
            int j = i - 1;

            while (j >= 0 && result.Get(j) > key) {
                result.Set(j + 1, result.Get(j));
                --j;
            }

            result.Set(j + 1, key);
        }

        return result;
    }

    std::string GetName() const override {
        return "Insertion Sort";
    }
};