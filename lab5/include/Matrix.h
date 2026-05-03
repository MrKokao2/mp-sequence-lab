#pragma once

#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

class Matrix {
public:
    Matrix() : rows_(0), cols_(0) {}

    Matrix(int rows, int cols)
            : rows_(rows), cols_(cols), data_(static_cast<size_t>(rows) * cols, 0.0) {}

    int Rows() const { return rows_; }
    int Cols() const { return cols_; }

    double& At(int i, int j) {
        return data_[static_cast<size_t>(i) * cols_ + j];
    }

    double At(int i, int j) const {
        return data_[static_cast<size_t>(i) * cols_ + j];
    }

    void FillRandom(double minVal = 0.0, double maxVal = 10.0, unsigned seed = 42) {
        std::mt19937 generator(seed);
        std::uniform_real_distribution<double> distribution(minVal, maxVal);
        for (auto& v : data_) {
            v = distribution(generator);
        }
    }

    void FillIndex() {
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                At(i, j) = static_cast<double>(i + j);
            }
        }
    }

    bool ApproxEqual(const Matrix& other, double tolerance = 1e-6) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            return false;
        }
        for (size_t i = 0; i < data_.size(); ++i) {
            double diff = data_[i] - other.data_[i];
            if (diff < 0) diff = -diff;
            if (diff > tolerance) return false;
        }
        return true;
    }

    void Print(std::ostream& os = std::cout) const {
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                os << At(i, j) << " ";
            }
            os << "\n";
        }
    }

private:
    int rows_;
    int cols_;
    std::vector<double> data_;
};
