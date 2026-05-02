#pragma once

#include "Sequence.h"
#include <vector>
#include <stdexcept>
#include <algorithm>

template<typename TElement>
class ArraySequence : public Sequence<TElement> {
private:
    std::vector<TElement> data;

    void checkIndex(int index) const {
        if (index < 0 || index >= getLength()) {
            throw std::out_of_range("Index out of range");
        }
    }

public:
    int getLength() const override {
        return static_cast<int>(data.size());
    }

    bool getIsEmpty() const override {
        return data.empty();
    }

    TElement Get(int index) const override {
        checkIndex(index);
        return data[index];
    }

    TElement GetFirst() const override {
        return Get(0);
    }

    TElement GetLast() const override {
        return Get(getLength() - 1);
    }

    void Append(TElement item) override {
        data.push_back(item);
    }

    void Prepend(TElement item) override {
        data.insert(data.begin(), item);
    }

    void InsertAt(int index, TElement item) override {
        if (index < 0 || index > getLength()) {
            throw std::out_of_range("Index out of range");
        }
        data.insert(data.begin() + index, item);
    }

    void Remove(TElement item) override {
        auto it = std::find(data.begin(), data.end(), item);
        if (it != data.end()) {
            data.erase(it);
        }
    }

    ArraySequence<TElement> GetSubsequence(int startIndex, int endIndex) const {
        checkIndex(startIndex);
        checkIndex(endIndex);

        if (startIndex > endIndex) {
            throw std::out_of_range("Invalid subsequence range");
        }

        ArraySequence<TElement> result;

        for (int i = startIndex; i <= endIndex; ++i) {
            result.Append(data[i]);
        }

        return result;
    }
};