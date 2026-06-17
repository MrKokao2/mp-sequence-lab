#pragma once
#include <vector>
#include <functional>

template<typename T, typename Key>
int BinarySearch(const std::vector<T>& data, Key key, std::function<Key(const T&)> getter) {
    if (data.empty())
        return -1;

    int left = 0;
    int right = static_cast<int>(data.size()) - 1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (getter(data[mid]) == key)
            return mid;
        else if (getter(data[mid]) < key)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}