#include "Examples.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

namespace {

float ExpectedDotProduct(int n) {
    float sum = 0.0f;
    for (int i = 0; i < n; ++i) {
        sum += static_cast<float>(i) * static_cast<float>(i);
    }
    return sum;
}

bool ApproxEqual(float a, float b, float tolerance = 1e-3f) {
    return std::fabs(a - b) <= tolerance * std::max(1.0f, std::fabs(a));
}

void TestDotProductFor() {
    int n = 100;
    float result = omp_examples::Example2_DotProductFor(n, 2);
    assert(ApproxEqual(result, ExpectedDotProduct(n)));
}

void TestDotProductSubroutine() {
    int n = 100;
    float result = omp_examples::Example3_DotProductSubroutine(n, 2);
    assert(ApproxEqual(result, ExpectedDotProduct(n)));
}

void TestVectorSumSections() {
    int n = 50;
    std::vector<float> result(n, 0.0f);
    omp_examples::Example4_VectorSumSections(n, 2, result.data());

    for (int i = 0; i < n; ++i) {
        float expected = static_cast<float>(2 * i);
        assert(ApproxEqual(result[i], expected));
    }
}

void TestMatrixVector() {
    int m = 10;
    std::vector<float> result(m, 0.0f);
    omp_examples::Example5_MatrixVector(m, 2, result.data());

    for (int i = 0; i < m; ++i) {
        float expected = 0.0f;
        for (int j = 0; j < m; ++j) {
            expected += static_cast<float>(j + 1) * static_cast<float>(j + 1);
        }
        assert(ApproxEqual(result[i], expected));
    }
}

}

int main() {
    TestDotProductFor();
    TestDotProductSubroutine();
    TestVectorSumSections();
    TestMatrixVector();

    std::cout << "All lab3 tests passed\n";
    return 0;
}
