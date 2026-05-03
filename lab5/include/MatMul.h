#pragma once

#include "Matrix.h"

namespace matmul {

Matrix MultiplySerial(const Matrix& a, const Matrix& b);

Matrix MultiplyParallelFixedThreads(const Matrix& a, const Matrix& b, int threads);

Matrix MultiplyParallelAuto(const Matrix& a, const Matrix& b);

}
