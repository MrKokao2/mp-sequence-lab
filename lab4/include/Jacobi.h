#pragma once

#include <vector>

class Jacobi {
public:
    Jacobi(int n);

    void Init();

    double RunSerial(int itmax, double maxEps = 1e-8);
    double RunParallel(int itmax, double maxEps = 1e-8, int threads = 0);

    double Verify() const;

    int Size() const { return n_; }

private:
    int n_;
    std::vector<double> a_;
    std::vector<double> b_;

    inline int Idx(int i, int j, int k) const {
        return (i * n_ + j) * n_ + k;
    }

    void RelaxSerial();
    double ResidSerial();

    void RelaxParallel();
    double ResidParallel();
};
