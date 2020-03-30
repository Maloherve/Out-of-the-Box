#include <iostream>

#include "math/diagonal.hpp"
#include "math/diagonals.hpp"

using namespace qsim::math;

int main() {
    
    // step 1, initialize diagonal operators, yes
    
    // initialize diagonal functor
    auto A = diag_functor<double>([] (size_t m) { return m * 0.2; } );

    // initialize diagonal array
    diag_array<double, 5> B({0,1,3,4,5});

    // step 2, initialize a sub-diagonal operator, not fully passed

    diagonals<double,2> C({sdiag_entry<double>(0, 5.0), sdiag_entry<double>(2, 3.0)});
    //diagonals<double,2> D = make_diagonal<double>({0, 5.0}, {2, 3.0}); // TODO, not compiling

    // step 3, operate on each of them singularly

}
