#include "Examples.h"

#include <omp.h>
#include <cstdio>
#include <vector>

namespace omp_examples {

float Example2_DotProductFor(int n, int requestedThreads) {
    if (requestedThreads > 0) {
        omp_set_num_threads(requestedThreads);
    }

    std::vector<float> a(n), b(n);
    for (int i = 0; i < n; ++i) {
        a[i] = b[i] = static_cast<float>(i);
    }

    float sum = 0.0f;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }

    std::printf(" Sum = %f\n", sum);
    return sum;
}

}
