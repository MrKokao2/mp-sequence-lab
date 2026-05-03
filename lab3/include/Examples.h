#pragma once

namespace omp_examples {

void Example1_Hello(int requestedThreads);

float Example2_DotProductFor(int n, int requestedThreads);

float Example3_DotProductSubroutine(int n, int requestedThreads);

void Example4_VectorSumSections(int n, int requestedThreads, float* outC = nullptr);

void Example5_MatrixVector(int m, int requestedThreads, float* outC = nullptr);

}
