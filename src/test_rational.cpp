#include "my_rational.h"
#include "my_string.h"

#include <iostream>

using namespace std;

int main() {
    for (auto const &s : {"3.14", "10/14", "5"}) {
        cout << MyRational(s) << endl;
    }

    return 0;
}
