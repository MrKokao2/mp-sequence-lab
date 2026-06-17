#include "Jacobi.h"

#include <omp.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Row {
    int n;
    int itmax;
    double serial;
    double par2;
    double speedup2;
    double par8;
    double speedup8;
};

double TimeIt(const std::function<double()>& fn) {
    auto t0 = std::chrono::high_resolution_clock::now();
    fn();
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(t1 - t0).count();
}

double RunSerial(int n, int itmax, double& checksum) {
    Jacobi solver(n);
    solver.Init();
    double t = TimeIt([&]() {
        solver.RunSerial(itmax);
        return 0.0;
    });
    checksum = solver.Verify();
    return t;
}

double RunParallel(int n, int itmax, int threads, double& checksum) {
    Jacobi solver(n);
    solver.Init();
    double t = TimeIt([&]() {
        solver.RunParallel(itmax, 1e-8, threads);
        return 0.0;
    });
    checksum = solver.Verify();
    return t;
}

std::vector<Row> Benchmark(int k) {
    int sizes[] = {128 + k, 160 + k, 192 + k, 224 + k, 256 + k};
    int itmax = 10 + k % 4;

    std::vector<Row> rows;
    for (int n : sizes) {
        double cs0 = 0.0, cs2 = 0.0, cs8 = 0.0;

        double tSerial = RunSerial(n, itmax, cs0);
        double tPar2 = RunParallel(n, itmax, 2, cs2);
        double tPar8 = RunParallel(n, itmax, 8, cs8);

        rows.push_back(Row{n, itmax, tSerial, tPar2, tSerial / tPar2,
                           tPar8, tSerial / tPar8});
    }
    return rows;
}

void PrintTable(const std::vector<Row>& rows, int k) {
    int itmax = rows.empty() ? 0 : rows.front().itmax;
    std::printf("\nVariant k=%d, itmax=%d\n", k, itmax);
    std::printf("%-6s %-6s %-12s %-12s %-12s %-12s %-12s\n",
                "N", "itmax", "Serial,s", "Par2,s", "Speedup2", "Par8,s", "Speedup8");

    for (const Row& r : rows) {
        std::printf("%-6d %-6d %-12.4f %-12.4f %-12.3f %-12.4f %-12.3f\n",
                    r.n, r.itmax, r.serial, r.par2, r.speedup2, r.par8, r.speedup8);
    }
}

bool WriteCsv(const std::vector<Row>& rows, const std::string& path) {
    std::ofstream out(path);
    if (!out) {
        std::cerr << "Cannot open CSV file for writing: " << path << "\n";
        return false;
    }
    out << "N,itmax,serial_s,par2_s,speedup2,par8_s,speedup8\n";
    for (const Row& r : rows) {
        out << r.n << ',' << r.itmax << ',' << r.serial << ',' << r.par2 << ','
            << r.speedup2 << ',' << r.par8 << ',' << r.speedup8 << '\n';
    }
    return static_cast<bool>(out);
}

}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  " << argv[0] << " serial   <N> <itmax>\n";
        std::cout << "  " << argv[0] << " parallel <N> <itmax> [threads]\n";
        std::cout << "  " << argv[0] << " table    [k] [csv_path]\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "serial") {
        int n = (argc > 2) ? std::atoi(argv[2]) : 128;
        int itmax = (argc > 3) ? std::atoi(argv[3]) : 10;
        double cs = 0.0;
        double t = RunSerial(n, itmax, cs);
        std::printf("Serial: N=%d itmax=%d time=%.4fs S=%f\n", n, itmax, t, cs);
    } else if (mode == "parallel") {
        int n = (argc > 2) ? std::atoi(argv[2]) : 128;
        int itmax = (argc > 3) ? std::atoi(argv[3]) : 10;
        int threads = (argc > 4) ? std::atoi(argv[4]) : 2;
        double cs = 0.0;
        double t = RunParallel(n, itmax, threads, cs);
        std::printf("Parallel: N=%d itmax=%d threads=%d time=%.4fs S=%f\n",
                    n, itmax, threads, t, cs);
    } else if (mode == "table") {
        int k = (argc > 2) ? std::atoi(argv[2]) : 10;
        std::string csvPath = (argc > 3) ? argv[3] : "";

        std::vector<Row> rows = Benchmark(k);
        PrintTable(rows, k);

        if (!csvPath.empty()) {
            if (WriteCsv(rows, csvPath)) {
                std::printf("CSV written: %s\n", csvPath.c_str());
            } else {
                return 1;
            }
        }
    } else {
        std::cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }

    return 0;
}
