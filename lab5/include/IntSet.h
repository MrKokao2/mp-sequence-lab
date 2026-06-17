#pragma once

#include <cstddef>
#include <random>
#include <unordered_set>
#include <vector>

class IntSet {
public:
    IntSet() = default;

    explicit IntSet(const std::vector<int>& values) {
        for (int v : values) {
            if (lookup_.insert(v).second) {
                elems_.push_back(v);
            }
        }
    }

    bool Contains(int x) const { return lookup_.find(x) != lookup_.end(); }

    const std::vector<int>& Elements() const { return elems_; }

    std::size_t Size() const { return elems_.size(); }

    static IntSet Random(std::size_t n, int maxVal, unsigned seed) {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dist(1, maxVal);

        std::unordered_set<int> seen;
        seen.reserve(n * 2);
        std::vector<int> values;
        values.reserve(n);

        while (values.size() < n) {
            int v = dist(gen);
            if (seen.insert(v).second) {
                values.push_back(v);
            }
        }
        return IntSet(values);
    }

    static IntSet Union(const IntSet& a, const IntSet& b) {
        std::vector<int> r = a.elems_;
        for (int x : b.elems_) {
            if (!a.Contains(x)) r.push_back(x);
        }
        return IntSet(r);
    }

    static IntSet Intersection(const IntSet& a, const IntSet& b) {
        std::vector<int> r;
        for (int x : a.elems_) {
            if (b.Contains(x)) r.push_back(x);
        }
        return IntSet(r);
    }

    static IntSet Difference(const IntSet& a, const IntSet& b) {
        std::vector<int> r;
        for (int x : a.elems_) {
            if (!b.Contains(x)) r.push_back(x);
        }
        return IntSet(r);
    }

private:
    std::vector<int> elems_;
    std::unordered_set<int> lookup_;
};
