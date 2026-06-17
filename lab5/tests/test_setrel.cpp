#include "IntSet.h"
#include "SetRelations.h"

#include <cassert>
#include <iostream>

namespace {

void TestSmallExamples() {
    IntSet a({1, 2, 3, 4, 5});
    IntSet b({4, 5, 6, 7});

    {
        IntSet c = IntSet::Union(a, b);
        auto r = setrel::CheckRelationsSerial(a, b, c);
        assert(r.is_union);
        assert(!r.is_intersection);
        assert(!r.is_diff_ab);
        assert(!r.is_diff_ba);
    }
    {
        IntSet c({4, 5});
        auto r = setrel::CheckRelationsSerial(a, b, c);
        assert(!r.is_union);
        assert(r.is_intersection);
        assert(!r.is_diff_ab);
        assert(!r.is_diff_ba);
    }
    {
        IntSet c({1, 2, 3});
        auto r = setrel::CheckRelationsSerial(a, b, c);
        assert(r.is_diff_ab);
        assert(!r.is_diff_ba);
        assert(!r.is_union);
    }
    {
        IntSet c({6, 7});
        auto r = setrel::CheckRelationsSerial(a, b, c);
        assert(r.is_diff_ba);
        assert(!r.is_diff_ab);
    }
    {
        IntSet c({42});
        auto r = setrel::CheckRelationsSerial(a, b, c);
        assert(!r.is_union && !r.is_intersection &&
               !r.is_diff_ab && !r.is_diff_ba);
    }
}

void TestOrderAndDuplicatesIgnored() {
    IntSet a({3, 1, 2, 2, 1});
    IntSet b({2, 3, 4});
    IntSet c({4, 1, 2, 3, 3});

    auto r = setrel::CheckRelationsSerial(a, b, c);
    assert(r.is_union);
}

void TestEmptyIntersection() {
    IntSet a({1, 2, 3});
    IntSet b({4, 5, 6});
    IntSet empty;

    auto r = setrel::CheckRelationsSerial(a, b, empty);
    assert(r.is_intersection);
    assert(!r.is_diff_ab);
}

void TestParallelMatchesSerial() {
    for (unsigned seed = 1; seed <= 5; ++seed) {
        IntSet a = IntSet::Random(7, 50, seed);
        IntSet b = IntSet::Random(7, 50, seed + 100);
        IntSet c = IntSet::Union(a, b);

        auto serial = setrel::CheckRelationsSerial(a, b, c);

        for (int threads : {1, 2, 3, 4, 8}) {
            auto par = setrel::CheckRelationsParallel(a, b, c, threads);
            assert(par.is_union == serial.is_union);
            assert(par.is_intersection == serial.is_intersection);
            assert(par.is_diff_ab == serial.is_diff_ab);
            assert(par.is_diff_ba == serial.is_diff_ba);
        }
    }
}

void TestLargerRandom() {
    IntSet a = IntSet::Random(500, 5000, 7u);
    IntSet b = IntSet::Random(500, 5000, 9u);

    auto check = [&](const IntSet& c, bool u, bool i, bool dab, bool dba) {
        auto s = setrel::CheckRelationsSerial(a, b, c);
        auto p = setrel::CheckRelationsParallel(a, b, c, 4);
        assert(s.is_union == u && s.is_intersection == i &&
               s.is_diff_ab == dab && s.is_diff_ba == dba);
        assert(p.is_union == u && p.is_intersection == i &&
               p.is_diff_ab == dab && p.is_diff_ba == dba);
    };

    check(IntSet::Union(a, b), true, false, false, false);
    check(IntSet::Intersection(a, b), false, true, false, false);
    check(IntSet::Difference(a, b), false, false, true, false);
    check(IntSet::Difference(b, a), false, false, false, true);
}

void TestThreadDerivation() {
    int small = setrel::DeriveThreadCount(10, 10, 10);
    assert(small == 1);

    int big = setrel::DeriveThreadCount(1000000, 1000000, 2000000);
    assert(big >= 1);
}

}  // namespace

int main() {
    TestSmallExamples();
    TestOrderAndDuplicatesIgnored();
    TestEmptyIntersection();
    TestParallelMatchesSerial();
    TestLargerRandom();
    TestThreadDerivation();

    std::cout << "All lab5 tests passed\n";
    return 0;
}
