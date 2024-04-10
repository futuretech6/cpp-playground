#include <algorithm>
#include <cstddef>
#include <stdexcept>

template <typename T>
class MyVector {
  public:
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = T *;
    using const_pointer   = T const *;
    using reference       = T &;
    using const_reference = T const &;

    MyVector() : p(nullptr), vSize(0), vCapacity(0) {}
    MyVector(size_type const n) { MyVector(n, value_type()); }
    MyVector(size_type const n, value_type const val) { resize(n, val); }
    ~MyVector() {
        if (p) {
            delete[] p;
        }
    }

    size_type size() const { return vSize; }
    size_type capacity() const { return vCapacity; }
    bool empty() const { return vSize == 0; }
    reference front() const { return empty() ? throw std::out_of_range("") : p[0]; }
    reference back() const { return empty() ? throw std::out_of_range("") : p[vSize - 1]; }

    void resize(size_type const n) { resize(n, value_type()); }
    void resize(size_type const n, value_type const val) {
        if (n > vCapacity) {
            allocate(n);
        }
        vSize = n;
        std::fill(p, p + n, val);
    }
    void reserve(size_type n) { n < vSize ? throw std::out_of_range("") : allocate(n); }

    void push_back(const_reference val) {
        if (vSize == vCapacity) {
            allocate(vCapacity << 1);
        }
        p[vSize++] = val;
    }
    void pop_back() {
        if (vSize == 0) {
            throw std::out_of_range("");
        }
        if (vSize < vCapacity >> 1) {
            allocate(vCapacity >> 1);
        }
        vSize--;
    }

  private:
    pointer p           = nullptr;
    size_type vSize     = 0;
    size_type vCapacity = 0;

    /**
     * @brief re-allocate a space
     *
     * @param n should >= size
     *
     * @warning size will not be changed during allocate
     */
    void allocate(size_type n) {
        auto *new_p = new value_type[n];
        if (p) {
            std::copy(p, p + vSize, new_p);
            delete[] p;
        }
        p         = new_p;
        vCapacity = n;
    }
};

#include <iostream>

int main() {
    auto v = MyVector(3, 1);

    v.resize(4, 2);

    v.push_back(3);

    return 0;
}
