#include "Examples.h"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    int threads = 2;

    if (argc > 1) {
        threads = std::stoi(argv[1]);
    }

    int choice = -1;

    while (choice != 0) {
        std::cout << "\nLab3: OpenMP Examples (threads = " << threads << ")\n";
        std::cout << "1 - Example 1.1: Hello World from threads\n";
        std::cout << "2 - Example 1.2: Dot product (parallel for + reduction)\n";
        std::cout << "3 - Example 1.3: Dot product in subroutine\n";
        std::cout << "4 - Example 1.4: Vector sum via sections\n";
        std::cout << "5 - Example 1.5: Matrix-vector multiplication\n";
        std::cout << "6 - Set thread count\n";
        std::cout << "0 - Exit\n";
        std::cout << "Choice: ";

        std::cin >> choice;

        if (choice == 1) {
            omp_examples::Example1_Hello(threads);
        } else if (choice == 2) {
            omp_examples::Example2_DotProductFor(100, threads);
        } else if (choice == 3) {
            omp_examples::Example3_DotProductSubroutine(100, threads);
        } else if (choice == 4) {
            omp_examples::Example4_VectorSumSections(50, threads);
        } else if (choice == 5) {
            omp_examples::Example5_MatrixVector(10, threads);
        } else if (choice == 6) {
            std::cout << "Threads: ";
            std::cin >> threads;
        }
    }

    return 0;
}
