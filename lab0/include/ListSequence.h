#pragma once

#include "Sequence.h"
#include <list>
#include <stdexcept>
#include <algorithm>

template<typename TElement>
class ListSequence : public Sequence<TElement> {
private:
    std::list<TElement> data;

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

        auto it = data.begin();
        std::advance(it, index);
        return *it;
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
        data.push_front(item);
    }

    void InsertAt(int index, TElement item) override {
        if (index < 0 || index > getLength()) {
            throw std::out_of_range("Index out of range");
        }

        auto it = data.begin();
        std::advance(it, index);
        data.insert(it, item);
    }

    void Remove(TElement item) override {
        auto it = std::find(data.begin(), data.end(), item);
        if (it != data.end()) {
            data.erase(it);
        }
    }

    ListSequence<TElement> GetSubsequence(int startIndex, int endIndex) const {
        checkIndex(startIndex);
        checkIndex(endIndex);

        if (startIndex > endIndex) {
            throw std::out_of_range("Invalid subsequence range");
        }

        ListSequence<TElement> result;

        for (int i = startIndex; i <= endIndex; ++i) {
            result.Append(Get(i));
        }

        return result;
    }
};