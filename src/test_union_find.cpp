#include <iostream>

#include "union_find.h"

int main() {
    using namespace std;

    int n = 5;  // 节点数量
    UnionFind uf(n);

    uf.unite(0, 1);
    uf.unite(2, 3);
    uf.unite(0, 4);

    cout << "0 and 3 are in the same set: " << (uf.same(0, 3) ? "Yes" : "No") << endl;
    cout << "1 and 2 are in the same set: " << (uf.same(1, 2) ? "Yes" : "No") << endl;
    cout << "1 and 2 are in the same set: " << (uf.same(1, 4) ? "Yes" : "No") << endl;

    for (auto i = 0; i < n; i++) {
        cout << "uf.find(" << i << "): " << uf.find(i) << endl;
    }

    return 0;
}
