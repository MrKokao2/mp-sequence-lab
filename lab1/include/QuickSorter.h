#pragma once

#include "ISorter.h"

template<typename T>
class QuickSorter : public ISorter<T> {
private:
    void quickSort(ArraySequence<T>& sequence, int left, int right) {
        if (left >= right) {
            return;
        }

        T pivot = sequence.Get((left + right) / 2);
        int i = left;
        int j = right;

        while (i <= j) {
            while (sequence.Get(i) < pivot) {
                ++i;
            }

            while (sequence.Get(j) > pivot) {
                --j;
            }

            if (i <= j) {
                T temp = sequence.Get(i);
                sequence.Set(i, sequence.Get(j));
                sequence.Set(j, temp);
                ++i;
                --j;
            }
        }

        if (left < j) {
            quickSort(sequence, left, j);
        }

        if (i < right) {
            quickSort(sequence, i, right);
        }
    }

public:
    ArraySequence<T> Sort(const ArraySequence<T>& sequence) override {
        ArraySequence<T> result = sequence;

        if (!result.getIsEmpty()) {
            quickSort(result, 0, result.getLength() - 1);
        }

        return result;
    }

    std::string GetName() const override {
        return "Quick Sort";
    }
};