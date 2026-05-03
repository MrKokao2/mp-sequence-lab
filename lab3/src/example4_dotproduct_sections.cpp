#include "Examples.h"

#include <omp.h>
#include <cstdio>
#include <vector>

namespace omp_examples {

void Example4_VectorSumSections(int n, int requestedThreads, float* outC) {
    if (requestedThreads > 0) {
        omp_set_num_threads(requestedThreads);
    }

    std::vector<float> a(n), b(n), c(n, 0.0f);
    for (int i = 0; i < n; ++i) {
        a[i] = b[i] = static_cast<float>(i);
    }

    #pragma omp parallel shared(a, b, c)
    {
        int rank = omp_get_thread_num();
        std::printf("Thread %d starting...\n", rank);

        #pragma omp sections nowait
        {
            #pragma omp section
            for (int i = 0; i < n / 2; ++i) {
                c[i] = a[i] + b[i];
                std::printf("rank = %d i= %d c[i]= %f\n", rank, i, c[i]);
            }

            #pragma omp section
            for (int i = n / 2; i < n; ++i) {
                c[i] = a[i] + b[i];
                std::printf("rank = %d i= %d c[i]= %f\n", rank, i, c[i]);
            }
        }

        if (rank == 0) {
            int size = omp_get_num_threads();
            std::printf("Number of threads = %d\n", size);
        }
    }

    if (outC) {
        for (int i = 0; i < n; ++i) {
            outC[i] = c[i];
        }
    }
}

}
