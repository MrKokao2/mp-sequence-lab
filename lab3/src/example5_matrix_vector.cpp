#include "Examples.h"

#include <omp.h>
#include <cstdio>
#include <vector>

namespace omp_examples {

void Example5_MatrixVector(int m, int requestedThreads, float* outC) {
    if (requestedThreads > 0) {
        omp_set_num_threads(requestedThreads);
    }

    std::vector<std::vector<float>> A(m, std::vector<float>(m));
    std::vector<float> b(m), c(m, 0.0f);

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            A[i][j] = static_cast<float>(j + 1);
        }
        b[i] = static_cast<float>(i + 1);
    }

    std::printf("Matrix A and vector b:\n");
    for (int i = 0; i < m; ++i) {
        std::printf(" A[%d]= ", i);
        for (int j = 0; j < m; ++j) {
            std::printf("%.1f ", A[i][j]);
        }
        std::printf(" b[%d]= %.1f\n", i, b[i]);
    }

    #pragma omp parallel shared(A, b, c)
    {
        int rank = omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                c[i] += A[i][j] * b[j];
            }

            #pragma omp critical
            {
                std::printf(" rank= %d i= %d c[%d]=%.2f\n", rank, i, i, c[i]);
            }
        }
    }

    if (outC) {
        for (int i = 0; i < m; ++i) {
            outC[i] = c[i];
        }
    }
}

}
