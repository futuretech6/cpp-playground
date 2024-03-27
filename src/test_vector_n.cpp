#include "my_vector_n.h"

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
