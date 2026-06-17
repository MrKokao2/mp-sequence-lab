#include "BinarySearch.h"
#include "BinaryTree.h"
#include "HashTable.h"

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <random>

using Clock = std::chrono::high_resolution_clock;
using KeyValue = std::pair<int, int>;   // (key, value=индекс записи)

std::mt19937& Rng() {
    static std::mt19937 generator(std::random_device{}());
    return generator;
}

// Случайный набор пар, отсортированный по ключу (для бинарного поиска).
std::vector<KeyValue> GenerateDataset(int n) {
    std::vector<KeyValue> data;
    data.reserve(n);

    std::uniform_int_distribution<int> distribution(0, n * 10);
    for (int i = 0; i < n; ++i)
        data.push_back({distribution(Rng()), i});

    std::sort(data.begin(), data.end(),
              [](const KeyValue& a, const KeyValue& b) {
                  return a.first < b.first;
              });

    return data;
}

// Случайные запросы: половина — заведомо существующие ключи,
// половина — произвольные (могут отсутствовать).
std::vector<int> GenerateQueries(const std::vector<KeyValue>& data, int m) {
    std::vector<int> queries;
    queries.reserve(m);

    std::uniform_int_distribution<int> pick(0, static_cast<int>(data.size()) - 1);
    std::uniform_int_distribution<int> any(0, static_cast<int>(data.size()) * 10);
    std::uniform_int_distribution<int> coin(0, 1);

    for (int i = 0; i < m; ++i) {
        if (coin(Rng()))
            queries.push_back(data[pick(Rng())].first);
        else
            queries.push_back(any(Rng()));
    }

    return queries;
}

long long MicrosBetween(Clock::time_point a, Clock::time_point b) {
    return std::chrono::duration_cast<std::chrono::microseconds>(b - a).count();
}

void CompareSearch(const std::vector<KeyValue>& data, int m) {
    if (data.empty()) {
        std::cout << "Dataset is empty, generate it first (option 1)\n";
        return;
    }

    std::vector<int> queries = GenerateQueries(data, m);
    auto getKey = [](const KeyValue& p) { return p.first; };

    // --- Бинарный поиск по отсортированному массиву: O(log n) ---
    long long binFound = 0;
    auto b0 = Clock::now();
    for (int q : queries)
        if (BinarySearch<KeyValue, int>(data, q, getKey) != -1)
            ++binFound;
    auto b1 = Clock::now();

    // --- AVL-дерево: построение O(n log n), поиск O(log n) ---
    BinaryTree<int, int> tree;
    auto tb0 = Clock::now();
    for (const auto& p : data)
        tree.Insert(p.first, p.second);
    auto tb1 = Clock::now();

    long long treeFound = 0;
    auto t0 = Clock::now();
    for (int q : queries)
        if (tree.Find(q) != -1)
            ++treeFound;
    auto t1 = Clock::now();

    // --- Хеш-таблица: построение O(n), поиск O(1) в среднем ---
    HashTable<int, int> table(std::max(16, static_cast<int>(data.size())));
    auto hb0 = Clock::now();
    for (const auto& p : data)
        table.Insert(p.first, p.second);
    auto hb1 = Clock::now();

    long long hashFound = 0;
    auto h0 = Clock::now();
    for (int q : queries)
        if (table.Find(q) != -1)
            ++hashFound;
    auto h1 = Clock::now();

    std::cout << "\nDataset size: " << data.size()
              << ", queries: " << m << "\n";
    std::cout << "AVL tree height: " << tree.Height()
              << "  (degenerate BST would be " << data.size() << ")\n";
    std::cout << "-----------------------------------------------------------\n";
    std::cout << "Binary search : lookup " << MicrosBetween(b0, b1)
              << " us  (found " << binFound << ")\n";
    std::cout << "AVL tree      : build " << MicrosBetween(tb0, tb1)
              << " us, lookup " << MicrosBetween(t0, t1)
              << " us  (found " << treeFound << ")\n";
    std::cout << "Hash table    : build " << MicrosBetween(hb0, hb1)
              << " us, lookup " << MicrosBetween(h0, h1)
              << " us  (found " << hashFound << ")\n";
}

void FindKey(const std::vector<KeyValue>& data, int key) {
    auto getKey = [](const KeyValue& p) { return p.first; };

    int idx = BinarySearch<KeyValue, int>(data, key, getKey);

    BinaryTree<int, int> tree;
    for (const auto& p : data)
        tree.Insert(p.first, p.second);

    HashTable<int, int> table(std::max(16, static_cast<int>(data.size())));
    for (const auto& p : data)
        table.Insert(p.first, p.second);

    std::cout << "Binary search value: " << (idx == -1 ? -1 : data[idx].second) << "\n";
    std::cout << "AVL tree value     : " << tree.Find(key) << "\n";
    std::cout << "Hash table value   : " << table.Find(key) << "\n";
}

int main() {
    std::vector<KeyValue> data;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\nSearch Structures Lab\n";
        std::cout << "1 - Generate random dataset\n";
        std::cout << "2 - Compare search efficiency\n";
        std::cout << "3 - Find key in all structures\n";
        std::cout << "0 - Exit\n";
        std::cout << "Choice: ";

        std::cin >> choice;

        if (choice == 1) {
            int n;
            std::cout << "Size: ";
            std::cin >> n;

            data = GenerateDataset(n);
            std::cout << "Generated " << data.size() << " sorted key-value pairs\n";
        } else if (choice == 2) {
            if (data.empty()) {
                std::cout << "Generate dataset first (option 1)\n";
                continue;
            }

            int m;
            std::cout << "Number of queries: ";
            std::cin >> m;

            CompareSearch(data, m);
        } else if (choice == 3) {
            if (data.empty()) {
                std::cout << "Generate dataset first (option 1)\n";
                continue;
            }

            int key;
            std::cout << "Key: ";
            std::cin >> key;

            FindKey(data, key);
        }
    }

    return 0;
}
