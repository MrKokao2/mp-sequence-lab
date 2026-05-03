#include "MatMul.h"

#include <omp.h>

#include <stdexcept>

namespace matmul {

namespace {

void CheckShapes(const Matrix& a, const Matrix& b) {
    if (a.Cols() != b.Rows()) {
        throw std::invalid_argument("Matrix shapes do not match for multiplication");
    }
}

}

Matrix MultiplySerial(const Matrix& a, const Matrix& b) {
    CheckShapes(a, b);

    int n = a.Rows();
    int m = b.Cols();
    int k = a.Cols();

    Matrix c(n, m);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            double sum = 0.0;
            for (int p = 0; p < k; ++p) {
                sum += a.At(i, p) * b.At(p, j);
            }
            c.At(i, j) = sum;
        }
    }

    return c;
}

Matrix MultiplyParallelFixedThreads(const Matrix& a, const Matrix& b, int threads) {
    CheckShapes(a, b);

    if (threads <= 0) {
        throw std::invalid_argument("Thread count must be positive");
    }

    int n = a.Rows();
    int m = b.Cols();
    int k = a.Cols();

    Matrix c(n, m);

    #pragma omp parallel for num_threads(threads) schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            double sum = 0.0;
            for (int p = 0; p < k; ++p) {
                sum += a.At(i, p) * b.At(p, j);
            }
            c.At(i, j) = sum;
        }
    }

    return c;
}

Matrix MultiplyParallelAuto(const Matrix& a, const Matrix& b) {
    CheckShapes(a, b);

    int n = a.Rows();
    int m = b.Cols();
    int k = a.Cols();

    Matrix c(n, m);

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            double sum = 0.0;
            for (int p = 0; p < k; ++p) {
                sum += a.At(i, p) * b.At(p, j);
            }
            c.At(i, j) = sum;
        }
    }

    return c;
}

}
