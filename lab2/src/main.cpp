#include "Student.h"
#include "BinarySearch.h"
#include "BinaryTree.h"
#include "HashTable.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

int main() {
    std::vector<Student> students = {
            {"Ivan", 20, 101},
            {"Petr", 21, 102},
            {"Anna", 19, 101},
            {"Oleg", 22, 103}
    };

    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  return a.age < b.age;
              });

    int key = 21;

    auto start = std::chrono::high_resolution_clock::now();

    int idx = BinarySearch<Student, int>(students, key,
                                         [](const Student& s) { return s.age; });

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Binary search index: " << idx << "\n";
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " us\n";

    BinaryTree<int, int> tree;
    for (int i = 0; i < students.size(); i++) {
        tree.Insert(students[i].age, i);
    }

    std::cout << "Tree search: " << tree.Find(21) << "\n";

    HashTable<int, int> table;
    for (int i = 0; i < students.size(); i++) {
        table.Insert(students[i].age, i);
    }

    std::cout << "Hash search: " << table.Find(21) << "\n";

    return 0;
}