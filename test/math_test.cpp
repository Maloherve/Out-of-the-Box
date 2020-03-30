#include <iostream>

#include "math/diagonal.hpp"
#include "math/diagonals.hpp"
#include "math/composition.hpp"

using namespace qsim::math;

int main() {
    
    // step 1, initialize diagonal operators, yes
    
    // initialize diagonal functor
    auto A = diag_functor<double>([] (size_t m) { return m * 0.2; } );

    // initialize diagonal array
    diag_array<double, 5> B({0,1,2,3,4});

    // step 2, initialize a sub-diagonal operator, not fully passed
    
    // by constructor
    diagonals<double,2> C({sdiag_entry<double>(0, 5.0), sdiag_entry<double>(2, 5.0)});

    // by helper
    //diagonals<double,2> D = make_diagonal<double>({0, 5.0}, {2, 3.0}); // TODO, not compiling

    // step 3, operate on each of them singularly by multiplication
    A *= 5; // A = (0, 1, 2, 3, 4, ...)
    B *= 2; // B = (0, 2, 4, 6, 8)
    C *= 0.2; // C = (1, 0, 1, 0, 0, 0, ...)

    // step 4, usage of external multiplication

    auto E = A * 2; // E = (0, 2, 4, 6, 8, ...)
    auto F = B * 0.5; // F = (1, 2, 3, 4, 5)
    auto G = C * 10;  // G = (1, 0, 1, 0, 0, 0, ...)

    // step 5, build a composition using A, B, C
    
    // matrix = 0 * id + A + B + C 
    auto matrix = composition(0, std::move(A), std::move(B), std::move(C));

    // step 6, apply scalar multiplication and diagonal addiction
    
    matrix *= 4;    // apply integer multiplication
    matrix *= 1.0 / 4; // apply double multiplication
    matrix += 2;

    // step 7, apply to a vector
    
    std::array<double, 5> v = {1, 1, 1, 1, 1};
    auto w = matrix * v;
}
