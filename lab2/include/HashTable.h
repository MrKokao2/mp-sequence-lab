#pragma once
#include <vector>
#include <list>
#include <functional>

template<typename Key, typename Value>
class HashTable {
private:
    std::vector<std::list<std::pair<Key, Value>>> table;
    int capacity;

public:
    HashTable(int cap = 100) : capacity(cap) {
        table.resize(capacity);
    }

    int hash(Key key) {
        return std::hash<Key>{}(key) % capacity;
    }

    void Insert(Key key, Value value) {
        int idx = hash(key);
        table[idx].push_back({key, value});
    }

    Value Find(Key key) {
        int idx = hash(key);

        for (auto& pair : table[idx]) {
            if (pair.first == key)
                return pair.second;
        }

        return -1;
    }
};