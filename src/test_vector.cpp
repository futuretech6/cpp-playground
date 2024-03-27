
#include "my_vector.h"

#include <iostream>

int main() {
    auto v = MyVector(3, 1);

    v.resize(4, 2);

    v.push_back(3);

    return 0;
}
