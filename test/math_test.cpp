#include <iostream>

#include "math/diagonal.hpp"
#include "math/diagonals.hpp"
#include "math/composition.hpp"

using namespace qsim::math;

template <class V>
void print(const V& v) {

    using namespace std;
    
    cout << "(";
    for (const auto& elem : v)
        cout << elem << ", ";
    cout << '\b' << ")";
}

int main() {
    
    // step 1, initialize diagonal operators, yes
    
    // initialize diagonal functor
    auto A = diag_functor<double>([] (size_t m) { return m * 0.2; } );

    // initialize diagonal array
    diag_array<double, 5> B({0,1,2,3,4});

    // step 2, initialize a sub-diagonal operator, not fully passed
    
    // by constructor
    diagonals<double,2> C({sdiag_entry<double>(1, -5.0), sdiag_entry<double>(-1, -5.0)});

    // by helper
    //diagonals<double,2> D = make_diagonal<double>({0, 5.0}, {2, 3.0}); // TODO, not compiling

    // step 3, operate on each of them singularly by multiplication
    A *= 5; // A = (0, 1, 2, 3, 4, ...)
    B *= 2; // B = (0, 2, 4, 6, 8)
    C *= 0.2; // C = subdiag<-1>(-1.0) + subdiag<1>(-1.0)

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
    std::array<double, 5> w = matrix * v;

    // step 8, runtime test print vectors to various expressions of the identity
    std::cout << "v = ";
    print(v); 
    std::cout << std::endl;

    auto id = composition(1.0); // simple identity
    std::cout << "Simple: Id * v = ";
    print(id * v);
    std::cout << std::endl;

    auto id_2 = composition(1.0, diag_functor<double>([] (size_t m) { return 1.0; } )); 
    id_2 *= 0.5; // identity, diagonal functor version
    std::cout << "Diagonal functor: Id * v = ";
    print(id_2 * v);
    std::cout << std::endl;

    auto id_3 = composition(1.0, diag_array<double,5>({1, 1, 1, 1, 1})); 
    id_3 *= 0.5; // identity, diagonal array version
    std::cout << "Diagonal array: Id * v = ";
    print(id_3 * v);
    std::cout << std::endl;

    auto id_4 = composition(2.0, diag_functor<double>([] (size_t m) { return 1.0; } ), diag_array<double,5>({1, 1, 1, 1, 1})); 
    id_4 *= 0.25; // identity, diagonal mixed version
    std::cout << "Diagonal functor + Diagonal array: Id * v = ";
    print(id_4 * v);
    std::cout << std::endl;

    // identity, diagonals version
    auto id_5 = composition(0, diagonals<double,1>({sdiag_entry<double>(0, 1.0)})); 
    std::cout << "Diagonals: Id * v = ";
    print(id_5 * v);
    std::cout << std::endl;

    // identity, totally mixed version
    auto id_6 = composition(1.0, 
                            diag_array<double,5>({1, 1, 1, 1, 1}),
                            diag_functor<double>([] (size_t m) { return 1.0; }),
                            diagonals<double,1>({sdiag_entry<double>(0, 1.0)})); 
    id_6 *= 0.25; // identity, diagonal mixed version
    std::cout << "Mixed: Id * v = ";
    print(id_6 * v);
    std::cout << std::endl;

    // step 9, print the matrix columns

    /*
     * At this point matrix should be:
     * 2 id + A + B + C = subdiag<-1>(-1.0) + diag(2, 5, 8, 11, 14) + subdiag<1>(-1.0) 
     */

    std::array<double, 5> cols[5] = {{1, 0, 0, 0, 0},
                                     {0, 1, 0, 0, 0},
                                     {0, 0, 1, 0, 0},
                                     {0, 0, 0, 1, 0},
                                     {0, 0, 0, 0, 1}};
    
    for (int i = 0; i < 5; ++i) {
        std::cout << "Column " << i << ": ";
        print(matrix * cols[i]); 
        std::cout << std::endl;
    }

    // expected result: v = (2, -1, 0, 0, 0)
    std::cout << "Final result: w = ";
    print(w); 
    std::cout << std::endl;
}
