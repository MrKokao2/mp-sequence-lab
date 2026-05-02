#include "BubbleSorter.h"
#include "InsertionSorter.h"
#include "QuickSorter.h"

#include <chrono>
#include <iostream>
#include <random>
#include <vector>

ArraySequence<int> GenerateRandomSequence(int length) {
    ArraySequence<int> sequence;
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 100000);

    for (int i = 0; i < length; ++i) {
        sequence.Append(distribution(generator));
    }

    return sequence;
}

template<typename T>
long long MeasureSortTime(ISorter<T>& sorter, const ArraySequence<T>& sequence) {
    auto start = std::chrono::high_resolution_clock::now();
    sorter.Sort(sequence);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void CompareSorters(const ArraySequence<int>& sequence) {
    BubbleSorter<int> bubbleSorter;
    InsertionSorter<int> insertionSorter;
    QuickSorter<int> quickSorter;

    std::cout << "Bubble Sort: "
              << MeasureSortTime(bubbleSorter, sequence)
              << " ms\n";

    std::cout << "Insertion Sort: "
              << MeasureSortTime(insertionSorter, sequence)
              << " ms\n";

    std::cout << "Quick Sort: "
              << MeasureSortTime(quickSorter, sequence)
              << " ms\n";
}

int main() {
    ArraySequence<int> sequence;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\nSorting Algorithms Lab\n";
        std::cout << "1 - Manual input\n";
        std::cout << "2 - Generate random sequence\n";
        std::cout << "3 - Bubble Sort\n";
        std::cout << "4 - Insertion Sort\n";
        std::cout << "5 - Quick Sort\n";
        std::cout << "6 - Compare all sorters\n";
        std::cout << "7 - Print current sequence\n";
        std::cout << "0 - Exit\n";
        std::cout << "Choice: ";

        std::cin >> choice;

        if (choice == 1) {
            sequence = ArraySequence<int>();

            int length;
            std::cout << "Length: ";
            std::cin >> length;

            for (int i = 0; i < length; ++i) {
                int value;
                std::cout << "Item " << i << ": ";
                std::cin >> value;
                sequence.Append(value);
            }
        } else if (choice == 2) {
            int length;
            std::cout << "Length: ";
            std::cin >> length;

            sequence = GenerateRandomSequence(length);
            std::cout << "Sequence generated\n";
        } else if (choice == 3) {
            BubbleSorter<int> sorter;
            sequence = sorter.Sort(sequence);
            std::cout << "Sorted by Bubble Sort\n";
        } else if (choice == 4) {
            InsertionSorter<int> sorter;
            sequence = sorter.Sort(sequence);
            std::cout << "Sorted by Insertion Sort\n";
        } else if (choice == 5) {
            QuickSorter<int> sorter;
            sequence = sorter.Sort(sequence);
            std::cout << "Sorted by Quick Sort\n";
        } else if (choice == 6) {
            CompareSorters(sequence);
        } else if (choice == 7) {
            sequence.Print();
        }
    }

    return 0;
}