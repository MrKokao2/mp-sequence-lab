#include "SetRelations.h"

#include <omp.h>

#include <algorithm>
#include <vector>

namespace setrel {

namespace {

template <typename Pred>
bool AllOf(const std::vector<int>& xs, Pred pred, int threads) {
    const int n = static_cast<int>(xs.size());
    bool ok = true;

    if (threads >= 1) {
        #pragma omp parallel for reduction(&& : ok) \
                num_threads(threads) schedule(static)
        for (int i = 0; i < n; ++i) {
            if (!pred(xs[i])) ok = false;
        }
    } else {
        for (int i = 0; i < n; ++i) {
            if (!pred(xs[i])) ok = false;
        }
    }
    return ok;
}

Relations CheckCore(const IntSet& a, const IntSet& b, const IntSet& c,
                    int threads) {
    const auto& A = a.Elements();
    const auto& B = b.Elements();
    const auto& C = c.Elements();

    auto inA = [&](int x) { return a.Contains(x); };
    auto inB = [&](int x) { return b.Contains(x); };
    auto inC = [&](int x) { return c.Contains(x); };

    Relations r;

    r.is_union =
        AllOf(C, [&](int x) { return inA(x) || inB(x); }, threads) &&
        AllOf(A, inC, threads) &&
        AllOf(B, inC, threads);

    r.is_intersection =
        AllOf(C, [&](int x) { return inA(x) && inB(x); }, threads) &&
        AllOf(A, [&](int x) { return !inB(x) || inC(x); }, threads);

    r.is_diff_ab =
        AllOf(C, [&](int x) { return inA(x) && !inB(x); }, threads) &&
        AllOf(A, [&](int x) { return inB(x) || inC(x); }, threads);

    r.is_diff_ba =
        AllOf(C, [&](int x) { return inB(x) && !inA(x); }, threads) &&
        AllOf(B, [&](int x) { return inA(x) || inC(x); }, threads);

    return r;
}

}  // namespace

int DeriveThreadCount(std::size_t sizeA, std::size_t sizeB, std::size_t sizeC) {
    constexpr std::size_t kChunk = 100000;
    std::size_t total = sizeA + sizeB + sizeC;

    int hw = omp_get_max_threads();
    int byWork = static_cast<int>((total + kChunk - 1) / kChunk);

    int t = std::max(1, std::min(byWork, hw));
    return t;
}

Relations CheckRelationsSerial(const IntSet& a, const IntSet& b,
                               const IntSet& c) {
    return CheckCore(a, b, c, 0);
}

Relations CheckRelationsParallel(const IntSet& a, const IntSet& b,
                                 const IntSet& c, int threads) {
    if (threads <= 0) {
        threads = DeriveThreadCount(a.Size(), b.Size(), c.Size());
    }
    return CheckCore(a, b, c, threads);
}

}  // namespace setrel
