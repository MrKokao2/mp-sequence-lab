#include "BinarySearch.h"
#include "BinaryTree.h"
#include "HashTable.h"

#include <vector>
#include <cassert>
#include <iostream>

void TestBinarySearch() {
    std::vector<int> data = {1, 2, 3, 4, 5};
    auto id = [](int x) { return x; };

    // Внешние скобки обязательны: иначе запятая в <int, int> ломает макрос assert.
    assert((BinarySearch<int, int>(data, 1, id) == 0));
    assert((BinarySearch<int, int>(data, 3, id) == 2));
    assert((BinarySearch<int, int>(data, 5, id) == 4));
    assert((BinarySearch<int, int>(data, 10, id) == -1));  // нет в массиве

    std::vector<int> empty;
    assert((BinarySearch<int, int>(empty, 1, id) == -1));  // пустой массив
}

void TestBinaryTree() {
    BinaryTree<int, int> tree;
    for (int i = 0; i < 7; ++i)
        tree.Insert(i, i * 10);

    assert(tree.Find(0) == 0);
    assert(tree.Find(3) == 30);
    assert(tree.Find(6) == 60);
    assert(tree.Find(100) == -1);                         // нет ключа

    tree.Insert(3, 999);                                  // обновление значения
    assert(tree.Find(3) == 999);
    assert(tree.Count() == 7);                            // дубликат не добавился
}

void TestBinaryTreeBalanced() {
    // Ключевой тест: вставляем строго возрастающую последовательность.
    // Несбалансированный BST выродился бы в список высотой N.
    // AVL держит высоту ~log2(N).
    BinaryTree<int, int> tree;
    const int n = 1023;                                   // 2^10 - 1

    for (int i = 0; i < n; ++i)
        tree.Insert(i, i);

    assert(tree.Count() == n);
    for (int i = 0; i < n; ++i)
        assert(tree.Find(i) == i);                        // все ключи находятся

    // Для AVL высота <= 1.44 * log2(n+2) ~ 14. Главное — что дерево НЕ
    // выродилось в список (высота должна быть много меньше n).
    int h = tree.Height();
    assert(h <= 14);
    std::cout << "AVL height for " << n << " sorted inserts: " << h
              << " (degenerate BST would be " << n << ")\n";
}

void TestHashTable() {
    HashTable<int, int> table;
    for (int i = 0; i < 10; ++i)
        table.Insert(i, i * 7);

    assert(table.Find(0) == 0);
    assert(table.Find(5) == 35);
    assert(table.Find(9) == 63);
    assert(table.Find(42) == -1);                         // нет ключа
}

int main() {
    TestBinarySearch();
    TestBinaryTree();
    TestBinaryTreeBalanced();
    TestHashTable();

    std::cout << "All lab2 tests passed\n";
    return 0;
}
