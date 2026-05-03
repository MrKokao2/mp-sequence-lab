#include "Jacobi.h"

#include <omp.h>

#include <algorithm>
#include <cmath>
#include <cstdio>

Jacobi::Jacobi(int n)
        : n_(n),
          a_(static_cast<size_t>(n) * n * n, 0.0),
          b_(static_cast<size_t>(n) * n * n, 0.0) {}

void Jacobi::Init() {
    int n = n_;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                bool boundary = (i == 0 || i == n - 1 ||
                                 j == 0 || j == n - 1 ||
                                 k == 0 || k == n - 1);
                a_[Idx(i, j, k)] = boundary ? 0.0 : (4.0 + i + j + k);
                b_[Idx(i, j, k)] = 0.0;
            }
        }
    }
}

void Jacobi::RelaxSerial() {
    int n = n_;
    for (int i = 1; i <= n - 2; ++i) {
        for (int j = 1; j <= n - 2; ++j) {
            for (int k = 1; k <= n - 2; ++k) {
                b_[Idx(i, j, k)] = (
                        a_[Idx(i - 1, j, k)] + a_[Idx(i + 1, j, k)] +
                        a_[Idx(i, j - 1, k)] + a_[Idx(i, j + 1, k)] +
                        a_[Idx(i, j, k - 1)] + a_[Idx(i, j, k + 1)]) / 6.0;
            }
        }
    }
}

double Jacobi::ResidSerial() {
    int n = n_;
    double eps = 0.0;
    for (int i = 1; i <= n - 2; ++i) {
        for (int j = 1; j <= n - 2; ++j) {
            for (int k = 1; k <= n - 2; ++k) {
                double e = std::fabs(a_[Idx(i, j, k)] - b_[Idx(i, j, k)]);
                a_[Idx(i, j, k)] = b_[Idx(i, j, k)];
                if (e > eps) eps = e;
            }
        }
    }
    return eps;
}

void Jacobi::RelaxParallel() {
    int n = n_;
    #pragma omp parallel for schedule(static)
    for (int i = 1; i <= n - 2; ++i) {
        for (int j = 1; j <= n - 2; ++j) {
            for (int k = 1; k <= n - 2; ++k) {
                b_[Idx(i, j, k)] = (
                        a_[Idx(i - 1, j, k)] + a_[Idx(i + 1, j, k)] +
                        a_[Idx(i, j - 1, k)] + a_[Idx(i, j + 1, k)] +
                        a_[Idx(i, j, k - 1)] + a_[Idx(i, j, k + 1)]) / 6.0;
            }
        }
    }
}

double Jacobi::ResidParallel() {
    int n = n_;
    double eps = 0.0;

    #pragma omp parallel
    {
        double localEps = 0.0;
        #pragma omp for schedule(static) nowait
        for (int i = 1; i <= n - 2; ++i) {
            for (int j = 1; j <= n - 2; ++j) {
                for (int k = 1; k <= n - 2; ++k) {
                    double e = std::fabs(a_[Idx(i, j, k)] - b_[Idx(i, j, k)]);
                    a_[Idx(i, j, k)] = b_[Idx(i, j, k)];
                    if (e > localEps) localEps = e;
                }
            }
        }
        #pragma omp critical
        {
            if (localEps > eps) eps = localEps;
        }
    }

    return eps;
}

double Jacobi::RunSerial(int itmax, double maxEps) {
    double eps = 0.0;
    for (int it = 1; it <= itmax; ++it) {
        RelaxSerial();
        eps = ResidSerial();
        std::printf("it=%4d eps=%g\n", it, eps);
        if (eps < maxEps) break;
    }
    return eps;
}

double Jacobi::RunParallel(int itmax, double maxEps, int threads) {
    if (threads > 0) {
        omp_set_num_threads(threads);
    }

    double eps = 0.0;
    for (int it = 1; it <= itmax; ++it) {
        RelaxParallel();
        eps = ResidParallel();
        std::printf("it=%4d eps=%g\n", it, eps);
        if (eps < maxEps) break;
    }
    return eps;
}

double Jacobi::Verify() const {
    int n = n_;
    double s = 0.0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                s += a_[Idx(i, j, k)] * (i + 1) * (j + 1) * (k + 1) /
                     (static_cast<double>(n) * n * n);
            }
        }
    }
    return s;
}
