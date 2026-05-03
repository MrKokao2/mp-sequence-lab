#include "Examples.h"

#include <omp.h>
#include <cstdio>
#include <vector>

namespace omp_examples {

namespace {

float g_sum;
std::vector<float> g_a;
std::vector<float> g_b;

float dotprod() {
    int n = static_cast<int>(g_a.size());
    int rank = omp_get_thread_num();

    #pragma omp for reduction(+:g_sum)
    for (int i = 0; i < n; ++i) {
        g_sum += g_a[i] * g_b[i];
        std::printf(" rank = %d i=%d\n", rank, i);
    }

    return g_sum;
}

}

float Example3_DotProductSubroutine(int n, int requestedThreads) {
    if (requestedThreads > 0) {
        omp_set_num_threads(requestedThreads);
    }

    g_a.assign(n, 0.0f);
    g_b.assign(n, 0.0f);
    for (int i = 0; i < n; ++i) {
        g_a[i] = g_b[i] = static_cast<float>(i);
    }

    g_sum = 0.0f;

    #pragma omp parallel
    {
        dotprod();
    }

    std::printf("Sum = %f\n", g_sum);
    return g_sum;
}

}
