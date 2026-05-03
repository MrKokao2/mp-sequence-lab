#include "Jacobi.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace {

bool ApproxEqual(double a, double b, double tolerance = 1e-9) {
    return std::fabs(a - b) <= tolerance * std::max(1.0, std::fabs(a));
}

void TestSerialMatchesParallel() {
    int n = 32;
    int itmax = 5;

    Jacobi serial(n);
    serial.Init();
    serial.RunSerial(itmax);
    double sumSerial = serial.Verify();

    Jacobi parallel(n);
    parallel.Init();
    parallel.RunParallel(itmax, 1e-8, 2);
    double sumParallel = parallel.Verify();

    assert(ApproxEqual(sumSerial, sumParallel, 1e-9));
}

void TestInitBoundaryZero() {
    int n = 8;
    Jacobi j(n);
    j.Init();

    double s = j.Verify();
    assert(s > 0.0);
}

void TestSingleIteration() {
    int n = 16;
    Jacobi j(n);
    j.Init();
    double eps = j.RunSerial(1);
    assert(eps > 0.0);
}

}

int main() {
    TestInitBoundaryZero();
    TestSingleIteration();
    TestSerialMatchesParallel();

    std::cout << "All lab4 tests passed\n";
    return 0;
}
