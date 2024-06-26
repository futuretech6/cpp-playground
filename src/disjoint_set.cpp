#include <unordered_map>

template <class T>
class DisjointSet {
  private:
    std::unordered_map<T, T> parent;

  public:
    DisjointSet() {}

    void add_node(const T &node) {
        if (parent.find(node) == parent.end()) {
            parent[node] = node;
        }
    }

    T find_root(const T &node) {
        if (parent[node] == node) {
            return node;
        }
        // return find_root(parent[node]);  // no compression
        return parent[node] = find_root(parent[node]);  // compression
    }

    void connect_nodes(const T &a, const T &b) {
        T root_a = find_root(a);
        T root_b = find_root(b);
        if (root_a != root_b) {
            parent[root_b] = root_a;
        }
    }
};

#include <iostream>

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
