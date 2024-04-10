#include <iostream>
#include <stdexcept>

template <typename T, size_t N>
class MyVectorN {
  private:
    size_t vSize;
    size_t heapCap;
    T vStack[N];
    T *vHeap;

  public:
    // Constructor
    MyVectorN() : vSize(0), heapCap(N), vHeap(nullptr) {}

    // Destructor
    ~MyVectorN() {
        if (vSize > N) {
            delete[] vHeap;
        }
    }

    // Copy constructor
    MyVectorN(const MyVectorN &other) : vSize(other.vSize), heapCap(other.heapCap) {
        if (vSize <= N) {
            vStack = other.vStack;
            vHeap  = nullptr;
        } else {
            vHeap = new T[heapCap];
            for (size_t i = 0; i < vSize; ++i) {
                vHeap[i] = other.vHeap[i];
            }
        }
    }

    // Assignment operator
    MyVectorN &operator=(const MyVectorN &other) {
        if (this != &other) {
            if (vSize > N) {
                delete[] vHeap;
            }
            vSize   = other.vSize;
            heapCap = other.heapCap;
            if (vSize <= N) {
                vStack = other.vStack;
                vHeap  = nullptr;
            } else {
                vHeap = new T[heapCap];
                for (size_t i = 0; i < vSize; ++i) {
                    vHeap[i] = other.vHeap[i];
                }
            }
        }
        return *this;
    }

    // Push an element
    void push_back(const T &val) {
        if (vSize < heapCap) {
            if (vSize < N) {
                vStack[vSize++] = val;
            } else {
                vHeap[vSize++] = val;
            }
        } else {
            heapCap *= 2;
            T *new_heap = new T[heapCap];
            for (size_t i = 0; i < vSize; ++i) {
                new_heap[i] = (vSize <= N) ? vStack[i] : vHeap[i];
            }
            delete[] vHeap;
            vHeap          = new_heap;
            vHeap[vSize++] = val;
        }
    }

    // Pop an element
    void pop_back() {
        if (vSize == 0) {
            throw std::out_of_range("Cannot pop from an empty vector");
        }

        if (vSize <= N) {
            --vSize;
        } else {
            --vSize;
            if (vSize < heapCap / 2) {
                heapCap /= 2;
                T *new_heap = new T[heapCap];
                for (size_t i = 0; i < vSize; ++i) {
                    new_heap[i] = vHeap[i];
                }
                delete[] vHeap;
                vHeap = new_heap;
            }
        }
    }

    // Get the size of the vector
    size_t size() const { return vSize; }

    // Access elements by index
    T &operator[](size_t index) {
        if (index >= vSize) {
            throw std::out_of_range("Index out of range");
        }
        return (vSize <= N) ? vStack[index] : vHeap[index];
    }

    const T &operator[](size_t index) const {
        if (index >= vSize) {
            throw std::out_of_range("Index out of range");
        }
        return (vSize <= N) ? vStack[index] : vHeap[index];
    }
};

int main() {
    // Example usage
    MyVectorN<int, 3> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);  // Exceeds stack storage, dynamically allocated
    std::cout << "Size of vector: " << v.size() << std::endl;
    std::cout << "Fourth element: " << v[3] << std::endl;
    return 0;
}
