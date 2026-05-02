#include "BubbleSorter.h"
#include "InsertionSorter.h"
#include "QuickSorter.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

template<typename T>
bool IsSorted(const ArraySequence<T>& sequence) {
    for (int i = 1; i < sequence.getLength(); ++i) {
        if (sequence.Get(i - 1) > sequence.Get(i)) {
            return false;
        }
    }

    return true;
}

template<typename TSorter>
void TestSorter() {
    TSorter sorter;

    ArraySequence<int> empty;
    assert(sorter.Sort(empty).getLength() == 0);

    ArraySequence<int> one({42});
    assert(sorter.Sort(one).Get(0) == 42);

    ArraySequence<int> unsorted({5, 1, 4, 2, 8});
    assert(IsSorted(sorter.Sort(unsorted)));

    ArraySequence<int> sorted({1, 2, 3, 4, 5});
    assert(IsSorted(sorter.Sort(sorted)));

    ArraySequence<int> reversed({5, 4, 3, 2, 1});
    assert(IsSorted(sorter.Sort(reversed)));

    ArraySequence<int> duplicates({3, 1, 3, 2, 1});
    assert(IsSorted(sorter.Sort(duplicates)));
}

void TestArraySequence() {
    ArraySequence<int> sequence;

    assert(sequence.getLength() == 0);
    assert(sequence.getIsEmpty());

    sequence.Append(10);
    sequence.Append(20);

    assert(sequence.getLength() == 2);
    assert(sequence.Get(0) == 10);
    assert(sequence.Get(1) == 20);

    sequence.Set(1, 30);
    assert(sequence.Get(1) == 30);

    try {
        sequence.Get(-1);
        assert(false);
    } catch (const std::out_of_range&) {}

    try {
        sequence.Get(2);
        assert(false);
    } catch (const std::out_of_range&) {}
}

int main() {
    TestArraySequence();

    TestSorter<BubbleSorter<int>>();
    TestSorter<InsertionSorter<int>>();
    TestSorter<QuickSorter<int>>();

    std::cout << "All lab1 tests passed\n";
    return 0;
}