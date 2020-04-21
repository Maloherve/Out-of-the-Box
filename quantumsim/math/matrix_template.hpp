
namespace qsim::math {

// with boundary check
template<class T>
const T& basic_matrix<T>::at(size_t i, size_t j) const {
    if (i < rows_nb() && j < cols_nb())
       return (*this)(i,j);
    else
       throw std::out_of_range("Matrix index accedes size (basic_matrix::at) const");
}

template<class T>
T& basic_matrix<T>::at(size_t i, size_t j) {
    if (i < rows_nb() && j < cols_nb())
       return (*this)(i,j);
    else
       throw std::out_of_range("Matrix index accedes size (basic_matrix::at)");
}

template<class T>
basic_matrix<T>& basic_matrix<T>::operator=(const basic_matrix<T>& other) {
    const size_t N = std::min(rows_nb(), other.rows_nb());
    const size_t M = std::min(cols_nb(), other.cols_nb());

    for (size_t i(0); i < N; ++i) {
        for (size_t j(0); j < M; ++j)
            (*this)(i,j) = other(i,j);
    }
    return *this;
}

template<class T>
basic_matrix<T>& basic_matrix<T>::operator+=(const basic_matrix<T>& other) {
    const size_t N = std::min(rows_nb(), other.rows_nb());
    const size_t M = std::min(cols_nb(), other.cols_nb());

    for (size_t i(0); i < N; ++i) {
        for (size_t j(0); j < M; ++j)
            (*this)(i,j) += other(i,j);
    }
    return *this;
}

template<class T>
basic_matrix<T>& basic_matrix<T>::operator-=(const basic_matrix<T>& other) {
    const size_t N = std::min(rows_nb(), other.rows_nb());
    const size_t M = std::min(cols_nb(), other.cols_nb());

    for (size_t i(0); i < N; ++i) {
        for (size_t j(0); j < M; ++j)
            (*this)(i,j) -= other(i,j);
    }
    return *this;
}

template<class T>
basic_matrix<T>& basic_matrix<T>::operator*=(const T& scalar) {
    for (size_t i(0); i < rows_nb(); ++i) {
        for (size_t j(0); j < cols_nb(); ++j)
            (*this)(i,j) *= scalar;
    }
    return *this;
}

template<class T>
basic_matrix<T>& basic_matrix<T>::operator/=(const T& scalar) {
    for (size_t i(0); i < rows_nb(); ++i) {
        for (size_t j(0); j < cols_nb(); ++j)
            (*this)(i,j) /= scalar;
    }
    return *this;
}

template<class T>
bool basic_matrix<T>::operator==(const basic_matrix<T>& other) const {
    if(rows_nb() != other.rows_nb() || cols_nb() != other.cols_nb())
        return false;

    for (size_t i(0); i < rows_nb(); ++i) {
        for (size_t j(0); j < cols_nb(); ++j) {
            if ((*this)(i,j) != other(i,j))
                return false;
        }
    }
    return true;
}

template<class T>
bool basic_matrix<T>::operator!=(const basic_matrix<T>& other) const {
    return !(*this == other);
}

template<class T>
void basic_matrix<T>::swap(basic_matrix<T>& other) {
    // size check
    if (rows_nb() == other.rows_nb() && cols_nb() == other.cols_nb()) {
        for (size_t i(0); i < rows_nb(); ++i) {
            for (size_t j(0); j < cols_nb(); ++j)
                std::swap((*this)(i,j), other(i,j));
        }
    } else {
        throw std::invalid_argument("Swap is allowed only if the size corresponds (basic_matrix::swap)");
    }
}

/*
 * Vector access implementations
 */

template<class T>
const T& vector_access<T>::at(size_t j) const {
    if (j < size())
       return (*this)[j];
    else
        throw std::out_of_range("Vector index excedes size (vector_access::at) const");

}

template<class T>
T& vector_access<T>::at(size_t j) {
    if (j < size())
       return (*this)[j];
    else
        throw std::out_of_range("Vector index excedes size (vector_access::at)");
}

/*
 * Sub-matrix functions definitions
 */ 
template<class T>
submatrix<T>::submatrix(matrix<T>* _ref, std::pair<size_t, size_t> _rows, std::pair<size_t, size_t> _cols)
    : ref(_ref), standalone(false), rows(_rows), cols(_cols) {}
 

// copy the restriction in a standalone buffer
template<class T>
submatrix<T>::submatrix(const submatrix<T>& other) 
    : ref(new matrix<T>(other)), standalone(true), rows(other.rows), cols(other.cols) 
{
}

template<class T>
submatrix<T>::submatrix(submatrix<T>&& other) 
    : ref(other), standalone(other.standalone), rows(other.rows), cols(other.cols)
{
    other.ref = nullptr;
    other.standalone = false;
    // throw an error on submatrix::at access
    other.rows = std::pair<size_t,size_t>(0,0); // empty restruction
    other.cols = std::pair<size_t,size_t>(0,0);
}

template<class T>
submatrix<T>& submatrix<T>::operator=(submatrix&& other) {

    if (standalone)
        delete ref;

    ref = other.ref;
    standalone = other.standalone;

    rows = other.rows; 
    cols = other.cols;

    other.ref = nullptr;
     // dont' delete out of scope
    other.standalone = false;
    // empty submatrix
    other.rows = std::pair<size_t,size_t>(0,0); // empty restruction
    other.cols = std::pair<size_t,size_t>(0,0);
    return *this;
}
        


template<class T>
submatrix<T>::~submatrix() {
    if (standalone)
        delete ref;
}

template<class T>
const T& submatrix<T>::operator()(size_t i, size_t j) const {
    // careful using this function: no controls at all
    return (*ref)(i + rows.first, j + cols.first);
}

template<class T>
T& submatrix<T>::operator()(size_t i, size_t j) {
    // careful using this function: no controls at all
    return (*ref)(i + rows.first, j + cols.first);
}

/*
template<class T>
submatrix<T>::operator matrix<T>() const {
    return matrix<T>(*this); 
}


template<class T>
row_vector<T>::operator matrix<T>() const {
    return matrix<T>(*this); 
}

template<class T>
column_vector<T>::operator matrix<T>() const {
    return matrix(*this); 
}
*/

/*
 * matrix class implementations
 */

template<class T>
matrix<T>::matrix(size_t N, size_t M, T init) 
        : table(N, row(M, init)) {
    if (N == 0 || M == 0)
        throw std::out_of_range("(matrix::matrix) size zero");
}

template<class T>
matrix<T>::matrix(std::initializer_list<std::initializer_list<T>> list) 
            : table() {

   if (list.size() == 0)
       throw std::out_of_range("(matrix::matrix) size zero");

   table.reserve(list.size());
   const size_t M = list.begin()->size();
   
   for (const auto& entry : list) {
       if (entry.size() != M)
           throw std::out_of_range("(matrix::matrix) incoherent initializer list");

       table.push_back(entry);
   }
}

template<class T>
matrix<T>::matrix(const submatrix<T>& sub) 
        : matrix(sub.rows_nb(), sub.cols_nb()) {
    for (size_t i = 0; i < sub.rows_nb(); ++i) {
        for (size_t j = 0; j < sub.cols_nb(); ++j) {
            npdebug("i = ", i, ", j = ", j, ", at = ", sub.at(i,j))
            (*this)(i,j) = sub.at(i,j);
        }
    }
}

template<class T>
submatrix<T> matrix<T>::restrict(std::pair<size_t,size_t> rows, std::pair<size_t, size_t> cols) {
    // convert ::all value to all indices
    if (rows == all)
        rows.second = rows_nb();

    if (cols == all)
        cols.second = cols_nb();

    // adjust entries
    if (rows.first >= rows.second || rows.second > rows_nb())
        throw std::out_of_range("(matrix::restrict) row index exceded");
    if (cols.first >= cols.second || cols.second > cols_nb()) {
        throw std::out_of_range("(matrix::restrict) column index exceded");
    }

    // transform in size
    rows.second -= rows.first;
    cols.second -= cols.first;

    return submatrix<T>(this, rows, cols);
}

template<class T>
row_vector<T> matrix<T>::get_row(size_t i, std::pair<size_t, size_t> cols) {
    
    if (i >= rows_nb())
        throw std::out_of_range("(matrix::restrict) row index exceded");

    if (cols == all)
        cols.second = cols_nb();

    // adjust entries
    if (cols.first >= cols.second || cols.second > cols_nb()) {
        throw std::out_of_range("(matrix::restrict) column index exceded");
    }

    // transform in size
    cols.second -= cols.first;
    
    return row_vector<T>(this, i, cols);
}

template<class T>
column_vector<T> matrix<T>::get_column(size_t j, std::pair<size_t,size_t> rows) {

    if (j >= cols_nb())
        throw std::out_of_range("(matrix::restrict) column index exceded");

    // convert ::all value to all indices
    if (rows == all)
        rows.second = rows_nb();

    // adjust entries
    if (rows.first >= rows.second || rows.second > rows_nb()) {
        throw std::out_of_range("(matrix::restrict) row index exceded");
    }

    // transform in size
    rows.second -= rows.first;

    return column_vector<T>(this, j, rows);
}

template<class T>
void matrix<T>::swap_rows(std::pair<size_t, size_t> swp) {
    if (swp.first != swp.second)
        std::swap(table[swp.first], table[swp.second]);
}

template<class T>
void matrix<T>::swap_rows(std::pair<size_t, size_t> swp, std::pair<size_t, size_t> col_range) {
    if (swp.first != swp.second) {
        // initialize 2 submatrices
        auto first = restrict(single(swp.first) ,col_range);
        auto second = restrict(single(swp.second) ,col_range);
        
        // swap content
        first.swap(second);
    }
}

/*
 * Square matrix implementations
 */

template<class T>
square_matrix<T>::square_matrix(size_t N, T init)
    : matrix<T>(N, N, init) {}


template<class T>
square_matrix<T>::square_matrix(std::initializer_list<std::initializer_list<T>> list) {
    if (list.size() == 0)
        throw std::out_of_range("(square_matrix::square_matrix) size zero");

    matrix<T>::table.reserve(list.size());
    
    for (const auto& entry : list) {
        if (entry.size() != list.size()) // square condition
            throw std::out_of_range("(square_matrix::square_matrix) incoherent initializer list");

        matrix<T>::table.push_back(entry);
    }
}

template<class T>
square_matrix<T> square_matrix<T>::eye(size_t N) {
    square_matrix<T> out(N);

    for (size_t i = 0; i < out.size(); ++i)
        out.at(i,i) = 1;

    return out;
}

/*
 * Convertion tool
 */ 


template <typename R, typename T>
matrix<R> convert(const basic_matrix<T>& A, const std::function<R (T)>& operation) {
    matrix<R> out(A.rows_nb(), A.cols_nb());

    for (size_t i = 0; i < A.rows_nb(); ++i) {
        for (size_t j = 0; j < A.cols_nb(); ++j) {
            out(i,j) = operation(A(i,j));
        }
    }

    return out;
}

/*
 * Compute LU decomposition
 * A = intering square matrix
 * Requirements: size(), static_cast to square_matrix<T>
 */
template<typename T, class Matrix>
helper::LU_output<T, Matrix> LU_decomposition(const Matrix& A) {
    const size_t N = A.size();
    helper::LU_output<T, Matrix> out(A);
    
    for (size_t k = 0; k < N-1; ++k) {
        // max index
        auto maxind = helper::max( std::abs( out.U.get_column(k, {k, N}) ));
        size_t r = maxind.first;

        if (k > 0) 
            r += k;

        if (r != k) {
            // swap rows
            out.U.swap_rows({k, r});
            out.P.push_front({k, r});

            if (k > 2)
                out.L.swap_rows({k, r}, {0, k-1});
        }

        for (size_t i(k+1); i < N; ++i) {
            out.L.at(i,k) = out.U.at(i,k) / out.U.at(k,k);
            for (size_t j(k); j < N; ++j)
                out.U.at(i,j) -= out.L.at(i,k) * out.U.at(k,j);
        }
    }

    return out;
}

/*
 * Solve A * s = b system
 */ 
template<typename T, class Matrix, class Vector>
Vector solve(const Matrix& A, Vector b) {
    // compute LU decomposition
    auto lu = LU_decomposition<T>(A);
    
    // solve pivoting on b
    while (lu.P.size() > 0) {
        std::swap(b[lu.P.back().first], b[lu.P.back().second]);
        lu.P.pop();
    }

    // set x = U * s, solve L * x = y
    Vector x = helper::solve_lower(lu.L, b);

    // solve U * s = x
    return helper::solve_upper(lu.U, x);
}

}

/*
 * Operator overloading for matrix
 */

template<typename T, class Vector>
Vector operator*(const qsim::math::basic_matrix<T>& A, const Vector& x) {
    if (A.cols_nb() != x.size())
        throw std::out_of_range("( operator*(matrix<T>, Vector) ) invalid column size");
    
    // it's supposed to be an array-like
    Vector out(x);

    for (size_t k = 0; k < x.size(); ++k) {
        out[k] = 0;
        for (size_t j = 0; j < x.size(); ++j) {
            out[k] += A(k,j) * x[j];
        }
    }

    return out;
}

namespace qsim::math::helper {

    template<typename T>
    std::pair<size_t, size_t> max(const qsim::math::matrix<T>& mat) {
        // set to the minimal value
        T m = std::numeric_limits<T>::min();
        std::pair<size_t, size_t> result;

        // compute the maximum
        for (size_t i = 0; i < mat.rows_nb(); ++i) {
            for (size_t j = 0; j < mat.cols_nb(); ++j) {
                if (mat.at(i,j) > m) {
                    m = mat.at(i,j);
                    result = {i,j}; 
                }
            }
        }

        return result;
    }
    
    // gauss elimination of lower triangular
    template<typename T, class Vector>
    Vector solve_lower(const square_matrix<T> L, Vector y) {

        const size_t N = L.size();

        for (size_t k=0; k < N-1; ++k) {
            y[k] = y[k] /  L(k,k);
            for (size_t i=k+1; i < N; ++i)
                y[i] -= y[k] * L(i,k);
        }

        y[N-1] /= L(N-1,N-1);

        return y;
    }

    // gauss elimination of upper triangular
    template<typename T, class Vector>
    Vector solve_upper(const square_matrix<T> U, Vector x) {

        const size_t N = U.size();

        for (size_t k=N-1; k > 0; --k) {
            x[k] = x[k] /  U(k,k);
            for (size_t i=k-1; i >= 0; --i)
                x[i] -= x[k] * U(i,k);
        }

        x[0] /= U(0,0);

        return x;
    }
}

/*
 * std::abs overload
 */

namespace std {

    // element-wise absolute value
    template <typename T>
    qsim::math::matrix<double> abs(const qsim::math::basic_matrix<T>& A) {
        return qsim::math::convert<double,T>(A, [&] (T value) { return std::abs(value); });
    }
}

/*
 * External operator overloading
 */

/*
 * Operator+
 */ 
template<typename T>
const qsim::math::matrix<T> operator+(qsim::math::matrix<T> A, const qsim::math::basic_matrix<T>& B) {
    return A += B;
}

template<typename T>
const qsim::math::submatrix<T> operator+(qsim::math::submatrix<T> A, const qsim::math::basic_matrix<T>& B) {
    return A += B;
}

template<typename T>
const qsim::math::row_vector<T> operator+(qsim::math::row_vector<T> A, const qsim::math::basic_matrix<T>& B) {
    return A += B;
}

template<typename T>
const qsim::math::column_vector<T> operator+(qsim::math::column_vector<T> A, const qsim::math::basic_matrix<T>& B) {
    return A += B;
}

/*
 * Operator-
 */
template<typename T>
const qsim::math::matrix<T> operator-(qsim::math::matrix<T> A, const qsim::math::basic_matrix<T>& B) {
    return A -= B;
}

template<typename T>
const qsim::math::submatrix<T> operator-(qsim::math::submatrix<T> A, const qsim::math::basic_matrix<T>& B) {
    return A -= B;
}

template<typename T>
const qsim::math::row_vector<T> operator-(qsim::math::row_vector<T> A, const qsim::math::basic_matrix<T>& B) {
    return A -= B;
}

template<typename T>
const qsim::math::column_vector<T> operator-(qsim::math::column_vector<T> A, const qsim::math::basic_matrix<T>& B) {
    return A -= B;
}

/*
 * Operator*
 */
template<typename T>
const qsim::math::matrix<T> operator*(qsim::math::matrix<T> A, const T& scalar) {
    return A *= scalar;
}

template<typename T>
const qsim::math::matrix<T> operator*(const T& scalar, qsim::math::matrix<T> A) {
    return A *= scalar;
}


template<typename T>
const qsim::math::submatrix<T> operator*(qsim::math::submatrix<T> A, const T& scalar) {
    return A *= scalar;
}

template<typename T>
const qsim::math::submatrix<T> operator*(const T& scalar, qsim::math::submatrix<T> A) {
    return A *= scalar;
}


template<typename T>
const qsim::math::row_vector<T> operator*(qsim::math::row_vector<T> A, const T& scalar) {
    return A *= scalar;
}

template<typename T>
const qsim::math::row_vector<T> operator*(const T& scalar, qsim::math::row_vector<T> A) {
    return A *= scalar;
}


template<typename T>
const qsim::math::column_vector<T> operator*(qsim::math::column_vector<T> A, const T& scalar) {
    return A *= scalar;
}

template<typename T>
const qsim::math::column_vector<T> operator*(const T& scalar, qsim::math::column_vector<T> A) {
    return A *= scalar;
}

/*
 * Operator/
 */
template<typename T>
const qsim::math::matrix<T> operator/(qsim::math::matrix<T> A, const T& scalar) {
    return A /= scalar;
}

template<typename T>
const qsim::math::submatrix<T> operator/(qsim::math::submatrix<T> A, const T& scalar) {
    return A /= scalar;
}

template<typename T>
const qsim::math::row_vector<T> operator/(qsim::math::row_vector<T> A, const T& scalar) {
    return A /= scalar;
}

template<typename T>
const qsim::math::column_vector<T> operator/(qsim::math::column_vector<T> A, const T& scalar) {
    return A /= scalar;
}



