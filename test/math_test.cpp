#include <iostream>

#include "math/diagonal.hpp"
#include "math/diagonals.hpp"

using namespace qsim::math;

int main() {
    
    // step 1, initialize diagonal operators
    
    // initialize diagonal functor
    auto A = diag_functor<double>([] (size_t m) { return m * 0.2; } );

    // initialize diagonal array
    diag_array<double, 5> B({0,1,3,4,5});

    // step 2, initialize a sub-diagonal operator 

    auto C = make_diagonals({2, 3.0}, {0, 5.0});
}
