#include <vector>

class UnionFind {
  private:
    std::vector<int> parent;
    std::vector<int> rank;

  public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;  // 初始化每个节点的父节点为自身
        }
    }

    // 查找节点所在集合的根节点（路径压缩）
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // 路径压缩，直接指向根节点
        }
        return parent[x];
    }

    // 合并两个节点所在的集合（按秩合并）
    void unite(int x, int y) {
        int root_x = find(x);
        int root_y = find(y);
        if (root_x == root_y)
            return;

        if (rank[root_x] < rank[root_y]) {
            parent[root_x] = root_y;
        } else if (rank[root_x] > rank[root_y]) {
            parent[root_y] = root_x;
        } else {
            parent[root_x] = root_y;
            rank[root_y]++;
        }
    }

    // 判断两个节点是否属于同一个集合
    bool same(int x, int y) { return find(x) == find(y); }
};

#include <iostream>

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
