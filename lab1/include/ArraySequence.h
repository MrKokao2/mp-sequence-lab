#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>

template<typename T>
class ArraySequence {
private:
    std::vector<T> data;

public:
    ArraySequence() = default;

    ArraySequence(const std::vector<T>& items) : data(items) {}

    int getLength() const {
        return static_cast<int>(data.size());
    }

    bool getIsEmpty() const {
        return data.empty();
    }

    T Get(int index) const {
        if (index < 0 || index >= getLength()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void Set(int index, T value) {
        if (index < 0 || index >= getLength()) {
            throw std::out_of_range("Index out of range");
        }
        data[index] = value;
    }

    void Append(T item) {
        data.push_back(item);
    }

    void Print() const {
        for (int i = 0; i < getLength(); ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << "\n";
    }
};