#include <iostream>

#include "math/diagonal.hpp"
#include "math/diagonals.hpp"
#include "math/composition.hpp"

#include "grid/wave.hpp"

#include "math/matrix.hpp"

using namespace qsim;
using namespace qsim::grid;
using namespace qsim::math;

template <class Matrix, class Vector>
Vector solve_tridiagonal(const Matrix& A, Vector x);

int main() {
    
    // step 1, initialize diagonal operators, yes
    
    // initialize diagonal functor
    auto A = diag_functor<double>([] (size_t m) { return m * 0.2; } );

    // initialize diagonal array
    diag_array<double, 5> B({0,1,2,3,4});

    // step 2, initialize a sub-diagonal operator, not fully passed
    
    // by constructor
    diagonals<double,2> C(
        std::array<sdiag_entry<double>,2>{sdiag_entry<double>(1, -5.0), sdiag_entry<double>(-1, -5.0)}
        );

    // by helper
    //diagonals<double,2> D = make_diagonal<double>({0, 5.0}, {2, 3.0}); // TODO, not compiling

    // step 3, operate on each of them singularly by multiplication
    A *= 5.0; // A = (0, 1, 2, 3, 4, ...)
    B *= 2.0; // B = (0, 2, 4, 6, 8)
    C *= 0.2; // C = subdiag<-1>(-1.0) + subdiag<1>(-1.0)

    // step 4, usage of external multiplication

    auto E = A * 2.0; // E = (0, 2, 4, 6, 8, ...)
    auto F = B * 0.5; // F = (1, 2, 3, 4, 5)
    auto G = C * 10.0;  // G = subdiag<-1>(-10.0) + subdiag<1>(-10.0)

    // step 5, build a composition using A, B, C
    
    // matrix = 0 * id + A + B + C 
    auto _matrix = composition(0.0, std::move(A), std::move(B), std::move(C));

    // step 6, apply scalar multiplication and diagonal addiction
    
    _matrix *= 4;    // apply integer multiplication
    _matrix *= 1.0 / 4; // apply double multiplication
    _matrix += 2;

    // step 7, apply to a vector, test the vector, test the components singularly
    
    wave_vector v = {1, 1, 1, 1, 1};
    wave_vector w = _matrix * v;

    const_cast<const wave_vector&>(v)[1];

    using namespace std;
    cout << endl << "Step 7" << endl;
    cout << "v = " << v << endl;

    wave_vector v2 = {-1, -1, -1, -1, -1};

    cout << "v + v2 = " << (v + v2) << endl;

    cout << "E * v" << (E * v) << endl;
    cout << "F * v" << (F * v) << endl;
    cout << "G * v" << (G * v) << endl;

    // step 8, runtime test print vectors to various expressions of the identity
    cout << "Step 8" << endl;

    auto id = composition(1.0); // simple identity
    cout << "Simple: Id * v = " << (id * v) << endl;

    auto id_2 = composition(1.0, diag_functor<double>([] (size_t m) { return 1.0; } )); 
    id_2 *= 0.5; // identity, diagonal functor version
    cout << "Diagonal functor: Id * v = " << (id_2 * v) << endl;

    auto id_3 = composition(1.0, diag_array<double,5>({1, 1, 1, 1, 1})); 
    id_3 *= 0.5; // identity, diagonal array version
    std::cout << "Diagonal array: Id * v = " << (id_3 * v) << endl;

    auto id_4 = composition(2.0, diag_functor<double>([] (size_t m) { return 1.0; } ), diag_array<double,5>({1, 1, 1, 1, 1})); 
    id_4 *= 0.25; // identity, diagonal mixed version
    std::cout << "Diagonal functor + Diagonal array: Id * v = " << (id_4 * v) << endl;

    // identity, diagonals version
    auto id_5 = composition(0, diagonals<double,1>({sdiag_entry<double>(0, 1.0)})); 
    std::cout << "Diagonals: Id * v = " << (id_5 * v) << endl;

    // identity, totally mixed version
    auto id_6 = composition(1.0, 
                            diag_array<double,5>({1, 1, 1, 1, 1}),
                            diag_functor<double>([] (size_t m) { return 1.0; }),
                            diagonals<double,1>({sdiag_entry<double>(0, 1.0)})); 
    id_6 *= 0.25; // identity, diagonal mixed version
    std::cout << "Mixed: Id * v = " << (id_6 * v) << endl;

    // step 9, print the matrix columns
    cout << endl << "Step 9" << endl;

    /*
     * At this point matrix should be:
     * 2 id + A + B + C = subdiag<-1>(-1.0) + diag(2, 5, 8, 11, 14) + subdiag<1>(-1.0) 
     */

    wave_vector cols_5[5] = {{1, 0, 0, 0, 0},
                         {0, 1, 0, 0, 0},
                         {0, 0, 1, 0, 0},
                         {0, 0, 0, 1, 0},
                         {0, 0, 0, 0, 1}};

#define PRINT_MATRIX(T, size, buffer)  \
    for (int i = 0; i < size; ++i) \
        std::cout << "Column " << i << ": " << (T * buffer[i]) << endl; \
    cout << endl;

    PRINT_MATRIX(_matrix, 5, cols_5)

    // single components

    // multiple of identity
    for (int i = 0; i < 5; ++i)
        std::cout << "Column " << i << ": " << (_matrix.get<0>() * cols_5[i]) << endl;
    cout << endl;

    // multiple of identity
    for (int i = 0; i < 5; ++i)
        std::cout << "Column " << i << ": " << (_matrix.get<1>() * cols_5[i]) << endl;
    cout << endl;

    // multiple of identity
    for (int i = 0; i < 5; ++i)
        std::cout << "Column " << i << ": " << (_matrix.get<2>() * cols_5[i]) << endl;
    cout << endl;

    // expected result: v = (2, -1, 0, 0, 0)
    std::cout << "Final result: w = " << w << endl;
    
    wave_vector cols_3[3] = {{1, 0, 0},
                             {0, 1, 0},
                             {0, 0, 1}};
    /*
     * Matrix test
     */
    /*std::cout << endl << "Starting matrix test" << w << endl << endl;

    matrix<double> mat(5, 5);
    auto I = square_matrix<double>::eye(5);

    cout << "Null: " << (mat * w) << endl; 
    cout << "Identity: " << (I * w) << endl;

    // submatrix
    auto I3 = I({0, 3}, {0, 3}); // 3x3 block
    cout << "I3 size: " << I3.cols_nb() << endl;
    try {
        cout << "Identity crash: " << (I3 * w) << endl;
    } catch (const std::exception& e) {
        cout << "Expected error: " << e.what() << endl;
    }

    wave_vector y = {1, 2, 1};
    cout << "Identity non-crash: " << (I3 * y) << endl;

    cout << "LU decomposition test" << endl;

    // LU decomposition of the identity
    auto lu = LU_decomposition<double>(I); 

    cout << "Identity L: " << (lu.L * w) << endl;
    cout << "Identity U: " << (lu.U * w) << endl;

    // LU decomposition of another matrix
    square_matrix<double> prob_mat = {{1, 2, 3}, {2, 4, 9}, {4, -3, 1}};
    PRINT_MATRIX(prob_mat, 3, cols_3)

    auto lu_prob = LU_decomposition<double>(prob_mat); 

    PRINT_MATRIX(lu_prob.L, 3, cols_3)
    PRINT_MATRIX(lu_prob.U, 3, cols_3)

    // arthmetic operations
    
    // copy
    auto prob_mat_cpy = prob_mat;
    cout << "Matrix copy: " << endl;
    PRINT_MATRIX(prob_mat, 3, cols_3)

    prob_mat_cpy += (prob_mat * 2.0);

    square_matrix<double>& prob_mat_ref = (prob_mat *= 3.0);
    cout << "Matrix multiplication by 3.0: " << endl;
    PRINT_MATRIX(prob_mat, 3, cols_3)
    
    cout << "Matrix addiction by (mult * 2): " << endl;
    PRINT_MATRIX(prob_mat_cpy, 3, cols_3)

    // submatrix move and copy
    cout << "Matrix and submatrix move/copy test: " << endl;
    {
        cout << "Dependent submatrix" << endl;
        auto prob_col = prob_mat.get_column(0); // dependent, no delete

        cout << "Independent submatrix" << endl;
        column_vector<double> prob_col_copy(prob_col); // dependent, delete

        cout << "Change the independent (* 2.0) clone" << endl;
        prob_col_copy *= 2.0;

        cout << "Copy assignment submatrix" << endl;
        prob_col = prob_col_copy;

        cout << "First column should be (* 2.0)" << endl;
        PRINT_MATRIX(prob_mat, 3, cols_3)
    }
*/

    // tridiagonal test
    //diagonals<double> tridiag = {{2, -1, 0}, {-1, 2, -1}, {0, -1, 2}};
    diagonals<double,3> tridiag({
        sdiag_entry(-1, -1.0), sdiag_entry(0, 2.0), sdiag_entry(1, -1.0)}
        );
    PRINT_MATRIX(tridiag, 3, cols_3)
    wave_vector b = {1, 0, 0};
    
    auto xw = solve_tridiagonal(tridiag, b);

    npdebug("x = ", xw)
}

template <class Matrix, class Vector>
Vector solve_tridiagonal(const Matrix& A, Vector x) {
	Vector new_diag(x);

    new_diag[0] = A.at(0,0);
    
    // eliminate lower diag    
    for(std::size_t i(1); i < x.size(); ++i) {

	    wave_t pivot = A.at(i, i-1) / new_diag[i-1];
	    x[i] -= x[i-1] * pivot;
	    new_diag[i] = A.at(i,i) - A.at(i-1, i) * pivot;
	}

    // eliminate upper diag    
	for(std::size_t i(x.size()-1); i > 0; --i) {
	    x[i] /= new_diag[i];
        x[i-1] -= A.at(i-1, i) * x[i];
	}
    
    x[0u] /= new_diag[0u];
    return x;
}
