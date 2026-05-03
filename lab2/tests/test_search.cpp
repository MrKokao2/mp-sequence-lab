#include "BinarySearch.h"
#include <vector>
#include <cassert>

int main() {
    std::vector<int> data = {1,2,3,4,5};

    int idx = BinarySearch<int,int>(data, 3, [](int x){ return x; });

    assert(idx == 2);

    int notFound = BinarySearch<int,int>(data, 10, [](int x){ return x; });

    assert(notFound == -1);

    return 0;
}