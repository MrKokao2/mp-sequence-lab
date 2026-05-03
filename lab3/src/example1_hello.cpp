#include "Examples.h"

#include <omp.h>
#include <cstdio>

namespace omp_examples {

void Example1_Hello(int requestedThreads) {
    if (requestedThreads > 0) {
        omp_set_num_threads(requestedThreads);
    }

    #pragma omp parallel default(none)
    {
        int rank = omp_get_thread_num();
        std::printf("Hello World from thread = %d\n", rank);

        if (rank == 0) {
            int size = omp_get_num_threads();
            std::printf("Number of threads = %d\n", size);
        }
    }
}

}
