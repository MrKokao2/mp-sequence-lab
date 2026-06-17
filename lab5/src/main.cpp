#include "IntSet.h"
#include "SetRelations.h"

#include <omp.h>

#include <chrono>
#include <cstdlib>
#include <fstream>
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

void PrintRelations(const setrel::Relations& r) {
    std::cout << "  C == A union B (A∪B): " << (r.is_union ? "да" : "нет") << "\n";
    std::cout << "  C == A inter B (A∩B): " << (r.is_intersection ? "да" : "нет") << "\n";
    std::cout << "  C == A \\ B          : " << (r.is_diff_ab ? "да" : "нет") << "\n";
    std::cout << "  C == B \\ A          : " << (r.is_diff_ba ? "да" : "нет") << "\n";
}

void RunDemo() {
    IntSet a({1, 2, 3, 4, 5});
    IntSet b({4, 5, 6, 7});

    std::cout << "A = {1,2,3,4,5}, B = {4,5,6,7}\n\n";

    struct Case {
        const char* name;
        IntSet c;
    };
    std::vector<Case> cases = {
        {"C = A∪B = {1..7}", IntSet::Union(a, b)},
        {"C = A∩B = {4,5}", IntSet::Intersection(a, b)},
        {"C = A\\B = {1,2,3}", IntSet::Difference(a, b)},
        {"C = B\\A = {6,7}", IntSet::Difference(b, a)},
        {"C = {42} (ничего)", IntSet({42})},
    };

    for (const auto& cs : cases) {
        std::cout << cs.name << ":\n";
        PrintRelations(setrel::CheckRelationsParallel(a, b, cs.c));
        std::cout << "\n";
    }
}

void RunAuto(int n) {
    IntSet a = IntSet::Random(n, 10 * n, 1u);
    IntSet b = IntSet::Random(n, 10 * n, 2u);
    IntSet c = IntSet::Union(a, b);

    int threads = setrel::DeriveThreadCount(a.Size(), b.Size(), c.Size());
    std::cout << "N=" << n << ", |A|=" << a.Size() << " |B|=" << b.Size()
              << " |C|=" << c.Size() << "\n";
    std::cout << "Потоков выбрано (из мощностей множеств): " << threads
              << " (доступно " << omp_get_max_threads() << ")\n\n";

    setrel::Relations r;
    double t = TimeMillis([&]() { r = setrel::CheckRelationsParallel(a, b, c); });
    PrintRelations(r);
    std::cout << "\nВремя: " << t << " ms\n";
}

void RunBench(int nmax, const std::string& csvPath) {
    std::vector<int> sizes = {nmax / 8, nmax / 4, nmax / 2, nmax};
    std::vector<int> threadCounts = {1, 2, 4, 8};

    std::cout << "\nN\tSerial(ms)";
    for (int t : threadCounts) {
        std::cout << "\tP-" << t << "(ms)\tS-" << t << "\tE-" << t;
    }
    std::cout << "\n";

    std::ofstream csv;
    if (!csvPath.empty()) {
        csv.open(csvPath);
        csv << "N,threads,serial_ms,parallel_ms,speedup,efficiency\n";
    }

    for (int n : sizes) {
        if (n <= 0) continue;
        IntSet a = IntSet::Random(n, 10 * n, 1u);
        IntSet b = IntSet::Random(n, 10 * n, 2u);
        IntSet c = IntSet::Union(a, b);

        double tSerial = TimeMillis(
            [&]() { setrel::CheckRelationsSerial(a, b, c); });

        std::cout << n << "\t" << tSerial;

        for (int t : threadCounts) {
            double tPar = TimeMillis(
                [&]() { setrel::CheckRelationsParallel(a, b, c, t); });
            double speedup = tSerial / tPar;
            double eff = speedup / t;
            std::cout << "\t" << tPar << "\t" << speedup << "\t" << eff;

            if (csv) {
                csv << n << ',' << t << ',' << tSerial << ',' << tPar << ','
                    << speedup << ',' << eff << '\n';
            }
        }
        std::cout << "\n";
    }

    if (csv) {
        std::cout << "CSV written: " << csvPath << "\n";
    }
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  " << argv[0] << " demo                 # наглядный пример на маленьких множествах\n";
        std::cout << "  " << argv[0] << " auto  <N>            # случайные A,B размера N, C=A∪B, потоки из мощностей\n";
        std::cout << "  " << argv[0] << " bench [Nmax] [csv]   # таблица время/ускорение/эффективность\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "demo") {
        RunDemo();
    } else if (mode == "auto") {
        int n = (argc > 2) ? std::atoi(argv[2]) : 100000;
        RunAuto(n);
    } else if (mode == "bench") {
        int nmax = (argc > 2) ? std::atoi(argv[2]) : 1600000;
        std::string csvPath = (argc > 3) ? argv[3] : "";
        RunBench(nmax, csvPath);
    } else {
        std::cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }

    return 0;
}
