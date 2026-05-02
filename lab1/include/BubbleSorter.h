#pragma once

#include "ISorter.h"

template<typename T>
class BubbleSorter : public ISorter<T> {
public:
    ArraySequence<T> Sort(const ArraySequence<T>& sequence) override {
        ArraySequence<T> result = sequence;

        for (int i = 0; i < result.getLength() - 1; ++i) {
            for (int j = 0; j < result.getLength() - i - 1; ++j) {
                if (result.Get(j) > result.Get(j + 1)) {
                    T temp = result.Get(j);
                    result.Set(j, result.Get(j + 1));
                    result.Set(j + 1, temp);
                }
            }
        }

        return result;
    }

    std::string GetName() const override {
        return "Bubble Sort";
    }
};