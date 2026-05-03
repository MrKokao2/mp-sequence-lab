#include "MatMul.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

namespace {

Matrix Identity(int n) {
    Matrix m(n, n);
    for (int i = 0; i < n; ++i) {
        m.At(i, i) = 1.0;
    }
    return m;
}

void TestIdentity() {
    Matrix a(4, 4);
    a.FillIndex();

    Matrix id = Identity(4);
    Matrix result = matmul::MultiplySerial(a, id);

    assert(result.ApproxEqual(a));
}

void TestKnownProduct() {
    Matrix a(2, 3);
    a.At(0, 0) = 1; a.At(0, 1) = 2; a.At(0, 2) = 3;
    a.At(1, 0) = 4; a.At(1, 1) = 5; a.At(1, 2) = 6;

    Matrix b(3, 2);
    b.At(0, 0) = 7;  b.At(0, 1) = 8;
    b.At(1, 0) = 9;  b.At(1, 1) = 10;
    b.At(2, 0) = 11; b.At(2, 1) = 12;

    Matrix expected(2, 2);
    expected.At(0, 0) = 58;  expected.At(0, 1) = 64;
    expected.At(1, 0) = 139; expected.At(1, 1) = 154;

    Matrix serial = matmul::MultiplySerial(a, b);
    Matrix par2 = matmul::MultiplyParallelFixedThreads(a, b, 2);
    Matrix par3 = matmul::MultiplyParallelFixedThreads(a, b, 3);
    Matrix autoP = matmul::MultiplyParallelAuto(a, b);

    assert(serial.ApproxEqual(expected));
    assert(par2.ApproxEqual(expected));
    assert(par3.ApproxEqual(expected));
    assert(autoP.ApproxEqual(expected));
}

void TestParallelMatchesSerialNonDivisibleSize() {
    Matrix a(7, 7), b(7, 7);
    a.FillRandom(0.0, 10.0, 1u);
    b.FillRandom(0.0, 10.0, 2u);

    Matrix serial = matmul::MultiplySerial(a, b);

    Matrix par3 = matmul::MultiplyParallelFixedThreads(a, b, 3);
    assert(par3.ApproxEqual(serial));

    Matrix par4 = matmul::MultiplyParallelFixedThreads(a, b, 4);
    assert(par4.ApproxEqual(serial));

    Matrix par8 = matmul::MultiplyParallelFixedThreads(a, b, 8);
    assert(par8.ApproxEqual(serial));

    Matrix autoP = matmul::MultiplyParallelAuto(a, b);
    assert(autoP.ApproxEqual(serial));
}

void TestShapeMismatch() {
    Matrix a(2, 3);
    Matrix b(4, 2);

    bool caught = false;
    try {
        matmul::MultiplySerial(a, b);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
}

}

int main() {
    TestIdentity();
    TestKnownProduct();
    TestParallelMatchesSerialNonDivisibleSize();
    TestShapeMismatch();

    std::cout << "All lab5 tests passed\n";
    return 0;
}
