
#include <iostream>

#include "disjoint_set.h"

int main() {
    DisjointSet<int> ds;
    for (auto i = 0; i < 10; i++) {
        ds.add_node(i);
    }
    ds.connect_nodes(1, 6);
    ds.connect_nodes(4, 6);

    for (auto i = 0; i < 10; i++) {
        std::cout << "Root of " << i << ": " << ds.find_root(i) << std::endl;
    }

    return 0;
}
