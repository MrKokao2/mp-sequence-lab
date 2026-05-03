#include "MatMul.h"

#include <omp.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

double TimeMillis(const std::function<void()>& fn) {
    auto t0 = std::chrono::high_resolution_clock::now();
    fn();
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

void PrintSpeedupTable(const std::vector<int>& sizes, const std::vector<int>& threadCounts) {
    std::cout << "\nN\tSerial(ms)";
    for (int t : threadCounts) {
        std::cout << "\tP-" << t << "(ms)\tS-" << t;
    }
    std::cout << "\tAuto(ms)\n";

    for (int n : sizes) {
        Matrix a(n, n), b(n, n);
        a.FillRandom(0.0, 10.0, 1u);
        b.FillRandom(0.0, 10.0, 2u);

        double tSerial = TimeMillis([&]() { matmul::MultiplySerial(a, b); });
        std::cout << n << "\t" << tSerial;

        for (int t : threadCounts) {
            double tPar = TimeMillis([&]() { matmul::MultiplyParallelFixedThreads(a, b, t); });
            double speedup = tSerial / tPar;
            std::cout << "\t" << tPar << "\t" << speedup;
        }

        double tAuto = TimeMillis([&]() { matmul::MultiplyParallelAuto(a, b); });
        std::cout << "\t" << tAuto << "\n";
    }
}

}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  " << argv[0] << " serial   <N>\n";
        std::cout << "  " << argv[0] << " parallel <N> <threads>      # variant 1: thread count is parameter\n";
        std::cout << "  " << argv[0] << " auto     <N>                # variant 2: thread count not a parameter\n";
        std::cout << "  " << argv[0] << " bench    [Nmax]\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "serial") {
        int n = (argc > 2) ? std::atoi(argv[2]) : 256;
        Matrix a(n, n), b(n, n);
        a.FillRandom(0.0, 10.0, 1u);
        b.FillRandom(0.0, 10.0, 2u);
        double t = TimeMillis([&]() { matmul::MultiplySerial(a, b); });
        std::cout << "Serial N=" << n << " time=" << t << " ms\n";
    } else if (mode == "parallel") {
        int n = (argc > 2) ? std::atoi(argv[2]) : 256;
        int threads = (argc > 3) ? std::atoi(argv[3]) : 2;
        Matrix a(n, n), b(n, n);
        a.FillRandom(0.0, 10.0, 1u);
        b.FillRandom(0.0, 10.0, 2u);
        double t = TimeMillis([&]() { matmul::MultiplyParallelFixedThreads(a, b, threads); });
        std::cout << "Parallel N=" << n << " threads=" << threads << " time=" << t << " ms\n";
    } else if (mode == "auto") {
        int n = (argc > 2) ? std::atoi(argv[2]) : 256;
        Matrix a(n, n), b(n, n);
        a.FillRandom(0.0, 10.0, 1u);
        b.FillRandom(0.0, 10.0, 2u);
        double t = TimeMillis([&]() { matmul::MultiplyParallelAuto(a, b); });
        std::cout << "Auto N=" << n << " (default OMP threads=" << omp_get_max_threads() << ") time=" << t << " ms\n";
    } else if (mode == "bench") {
        int nmax = (argc > 2) ? std::atoi(argv[2]) : 512;
        std::vector<int> sizes;
        for (int n = 64; n <= nmax; n *= 2) {
            sizes.push_back(n);
        }
        std::vector<int> threadCounts = {1, 2, 4};
        PrintSpeedupTable(sizes, threadCounts);
    } else {
        std::cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }

    return 0;
}
