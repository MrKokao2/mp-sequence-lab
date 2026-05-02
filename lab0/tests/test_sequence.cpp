#include "ArraySequence.h"
#include "ListSequence.h"

#include <cassert>
#include <stdexcept>
#include <iostream>

template<typename TSequence>
void runSequenceTest() {
    TSequence sequence;

    assert(sequence.getLength() == 0);
    assert(sequence.getIsEmpty());

    sequence.Append(23);

    assert(sequence.getLength() == 1);
    assert(sequence.GetFirst() == 23);
    assert(sequence.GetLast() == 23);
    assert(sequence.Get(0) == 23);

    try {
        sequence.Get(-1);
        assert(false);
    } catch (const std::out_of_range&) {}

    try {
        sequence.Get(1);
        assert(false);
    } catch (const std::out_of_range&) {}

    sequence.Append(43);

    assert(sequence.getLength() == 2);
    assert(sequence.GetFirst() == 23);
    assert(sequence.GetLast() == 43);
    assert(sequence.Get(0) == 23);
    assert(sequence.Get(1) == 43);

    sequence.Prepend(53);

    assert(sequence.getLength() == 3);
    assert(sequence.GetFirst() == 53);
    assert(sequence.GetLast() == 43);
    assert(sequence.Get(0) == 53);
    assert(sequence.Get(1) == 23);

    auto subsequence = sequence.GetSubsequence(1, 1);

    assert(subsequence.getLength() == 1);
    assert(subsequence.GetFirst() == 23);
    assert(subsequence.GetLast() == 23);
}

int main() {
    runSequenceTest<ArraySequence<int>>();
    runSequenceTest<ListSequence<int>>();

    std::cout << "All tests passed\n";
    return 0;
}