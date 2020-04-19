#pragma once

#include <vector>
#include <limits>
#include <deque>

#include "debug.hpp"

namespace qsim::math {

    template <typename T>
    class matrix;

    template <typename T>
    class square_matrix;

    template <typename T>
    class vector_access {
        public:

            virtual ~vector_access() {}
            
            virtual size_t size() const = 0;

            virtual const T& at(size_t j) const = 0;
            virtual T& at(size_t j) = 0;

            inline const T& operator[](size_t j) const {
                return at(j);
            }

            inline T& operator[](size_t j) {
                return at(j);
            }
    };

    /*
     * Element-wise sub-matrix wrapper
     */
    template <typename T>
    class submatrix {
    private:
        friend class matrix<T>;

        // reference to the matrix
        matrix<T>* ref;
        bool standalone;

    protected:
        // {begin index, size}
        const std::pair<size_t, size_t> rows, cols;

        // reference, {begin, end}, {begin, end}
        // construct a non-standalone submatrix
        submatrix(matrix<T>* _ref, std::pair<size_t, size_t> _rows, std::pair<size_t, size_t> _cols)
                : ref(_ref), standalone(false), rows(_rows), cols(_cols) {}
        
        // direct access without boundary check
        const T& _at(size_t i, size_t j) const;
        T& _at(size_t i, size_t j);

    public:
        submatrix() = delete;
        
        // move a submatrix and invalidate the other
        submatrix(submatrix&&);
        
        // construct a standalone copy
        // notice, this is the only way to construct a standalone submatrix
        submatrix(const submatrix&);

        // delete the instance if standalone
        virtual ~submatrix();

        /*
         * target index assigning
         */
        //submatrix& operator=(const submatrix&);
        submatrix& operator=(const matrix<T>&);
        
        // with boundary check
        const T& at(size_t i, size_t j) const;
        T& at(size_t i, size_t j);
        
        inline T& operator()(size_t i, size_t j) {
            return at(i,j);
        }

        inline const T& operator()(size_t i, size_t j) const {
            return at(i,j);
        }

        inline size_t rows_nb() const {
            return rows.second;
        }

        inline size_t cols_nb() const {
            return cols.second;
        }
        
        /*
         * Swap submatrices values
         */
        void swap(submatrix& other) {
            // size check
            if (rows_nb() == other.rows_nb() && 
                cols_nb() == other.cols_nb()) {

                for (size_t i(0); i < rows_nb(); ++i) {
                    for (size_t j(0); j < cols_nb(); ++j)
                        std::swap(at(i,j), other._at(i,j));
                }

            }
        }
        
        /*
         * implicit convertion to a matrix
         */
        operator matrix<T>() const;
    };

    template <typename T>
    class row_vector : public vector_access<T>, public submatrix<T> {
            friend class matrix<T>;

            row_vector(matrix<T>* _ref, size_t row, std::pair<size_t, size_t> _cols)
                : submatrix<T>(_ref, std::pair(row, row+1), _cols) {}

            using submatrix<T>::rows_nb;
            using submatrix<T>::cols_nb;
            using submatrix<T>::at;
            using submatrix<T>::_at;

            const T& _at(size_t j) const {
                return _at(this->rows.first, j);
            }

            T& _at(size_t j) {
                return _at(this->rows.first, j);
            }

        public:

            row_vector(const row_vector& other) : submatrix<T>(other) {}
            row_vector(row_vector&& other) : submatrix<T>(std::forward<row_vector>(other)) {}

            using submatrix<T>::operator=;

            operator matrix<T>() const;
            
            virtual size_t size() const override {
                return cols_nb();
            } 

            virtual const T& at(size_t j) const override {
                return at(this->rows.first, j);
            } 

            virtual T& at(size_t j) override {
                return at(this->rows.first, j);
            } 
    };

    template <typename T>
    class column_vector : public vector_access<T>, public submatrix<T> {
            friend class matrix<T>;

            column_vector(matrix<T>* _ref, size_t col, std::pair<size_t, size_t> _rows)
                : submatrix<T>(_ref, _rows, std::pair(col, col+1)) {}

            using submatrix<T>::rows_nb;
            using submatrix<T>::cols_nb;
            using submatrix<T>::at;
            using submatrix<T>::_at;

            const T& _at(size_t i) const {
                return _at(i, this->cols.first);
            }

            T& _at(size_t i) {
                return _at(i, this->cols.first);
            }

        public:

            column_vector(const column_vector& other) : submatrix<T>(other) {}
            column_vector(column_vector&& other) : submatrix<T>(std::forward<column_vector>(other)) {}

            using submatrix<T>::operator=;

            operator matrix<T>() const;
            
            virtual size_t size() const override {
                return rows_nb();
            } 

            virtual const T& at(size_t i) const override {
                return at(i, this->cols.first);
            } 

            virtual T& at(size_t i) override {
                return at(i, this->cols.first);
            } 
    };

    /*
     * General matrix class, dynamically allocated
     */
    template <typename T>
    class matrix {
    public:

        class row : public std::vector<T> {
            friend class square_matrix<T>;
            using std::vector<T>::reserve;
            using std::vector<T>::push_back;
            using std::vector<T>::pop_back;
        public:
            using std::vector<T>::vector;
        };

   protected:

        struct table_t : public std::vector<row> {
            using std::vector<row>::vector;
            using std::vector<row>::push_back;
            using std::vector<row>::pop_back;
            using std::vector<row>::reserve;
        };
    
        // rows table
        table_t table;
        
        // only if needed
        matrix() = default;

    public:

        matrix(size_t N, size_t M, T init = 0) 
            : table(N, row(M, init)) {
                if (N == 0 || M == 0)
                    throw std::out_of_range("(matrix::matrix) size zero");
            }

        matrix(std::initializer_list<std::initializer_list<T>> list) 
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

        
        // TODO check table deep copy
        matrix(const matrix&) = default;

        // move table
        matrix(matrix&& other) : table(std::move(other.table)) {}

        // construct copying a submatrix
        matrix(const submatrix<T>& sub) 
            : matrix(sub.rows_nb(), sub.cols_nb())
        {
            for (size_t i = 0; i < sub.rows_nb(); ++i) {
                for (size_t j = 0; j < sub.cols_nb(); ++j)
                    at(i,j) = sub.at(i,j);
            }
        }

        virtual ~matrix() = default;
        
        // no, boundary check
        inline const T& at(size_t i, size_t j) const {
            return table[i][j];
        }

        inline T& at(size_t i, size_t j) {
            return table[i][j];
        }

        inline size_t rows_nb() const {
            return table.size();
        }

        inline size_t cols_nb() const {
            return table[0].size();
        }
        
        /*
         * Create a sub-matrix with the given indexes
         */
        submatrix<T> restrict(std::pair<size_t,size_t> rows = all, std::pair<size_t, size_t> cols = all) {
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

        /*
         * Create a sub-matrix with a column indexing restriction
         */
        row_vector<T> get_row(size_t i, std::pair<size_t, size_t> cols) {

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
        
        /*
         * Row descriptor without a column restriction
         */
        row& get_row(size_t i) {
            return table[i];
        }

        const row& get_row(size_t i) const {
            return table[i];
        }

        /*
         * Create a sub-matrix with a row indexing restriction
         */
        column_vector<T> get_column(size_t j, std::pair<size_t,size_t> rows = all) {
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

        /*
         * swap the matrix internally
         */

        void swap_rows(std::pair<size_t, size_t> swp) {
            if (swp.first != swp.second)
                std::swap(table[swp.first], table[swp.second]);
        }
        
        // using a column range
        void swap_rows(std::pair<size_t, size_t> swp, std::pair<size_t, size_t> col_range) {
            if (swp.first != swp.second) {
                // initialize 2 submatrices
                auto first = restrict(single(swp.first) ,col_range);
                auto second = restrict(single(swp.second) ,col_range);
                
                // swap content
                first.swap(second);
            }
        }

        /*
         * access aliases
         */

        inline T& operator()(size_t i, size_t j) {
            return at(i,j);
        }

        inline const T& operator()(size_t i, size_t j) const {
            return at(i,j);
        }
        
        inline submatrix<T> operator()(std::pair<size_t,size_t> rows, std::pair<size_t,size_t> cols) {
            return restrict(rows, cols);
        }
        
        /*
         * helpers for restrict
         */
        static constexpr std::pair<size_t, size_t> all = {0, 0};

        // return a single line 
        static constexpr std::pair<size_t, size_t> single(size_t k) {
            return std::pair<size_t,size_t>(k, k+1);
        }
    };

    
    /*
     * Sub-matrix functions definitions
     */ 

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
        // throw an error on submatrix::at access
        other.rows = std::pair<size_t,size_t>(0,0); // empty restruction
        other.cols = std::pair<size_t,size_t>(0,0);
    }

    template<class T>
    submatrix<T>::~submatrix() {
        if (standalone)
            delete ref;
    }

    template<class T>
    const T& submatrix<T>::_at(size_t i, size_t j) const {
        // careful using this function: no controls at all
        return ref->at(i + rows.first, j + cols.first);
    }

    template<class T>
    T& submatrix<T>::_at(size_t i, size_t j) {
        // careful using this function: no controls at all
        return ref->at(i + rows.first, j + cols.first);
    }

    template<class T>
    const T& submatrix<T>::at(size_t i, size_t j) const {
        if (i < rows.second && j < cols.second)
           return _at(i,j);
        else
            throw std::out_of_range("Matrix index accedes size (submatrix::at) const");
    }

    template<class T>
    T& submatrix<T>::at(size_t i, size_t j) {
        if (i < rows.second && j < cols.second)
           return _at(i,j);
        else
            throw std::out_of_range("Matrix index accedes size (submatrix::at)");
    }

    template<class T>
    submatrix<T>::operator matrix<T>() const {
        return matrix(*this); 
    }

    template<class T>
    row_vector<T>::operator matrix<T>() const {
        return matrix(*this); 
    }

    template<class T>
    column_vector<T>::operator matrix<T>() const {
        return matrix(*this); 
    }

    /*template<class T>
    submatrix<T>& submatrix<T>::operator=(const submatrix<T>& other) {
        // dimension check 
        if (rows_nb() != other.rows_nb() || cols_nb() != other.cols_nb())
            throw std::invalid_argument("Dimension assert failed (submatrix::operator=)(submatrix)");

        for (size_t i(0); i < rows_nb(); ++i) {
            for (size_t j(0); j < cols_nb(); ++j)
                _at(i,j) = other._at(i,j);
        }

        return *this;
    }*/

    template<class T>
    submatrix<T>& submatrix<T>::operator=(const matrix<T>& other) {
        // dimension check 
        if (rows_nb() != other.rows_nb() || cols_nb() != other.cols_nb())
            throw std::invalid_argument("Dimension assert failed (submatrix::operator=)(matrix)");

        for (size_t i(0); i < rows_nb(); ++i) {
            for (size_t j(0); j < cols_nb(); ++j) {
                _at(i,j) = other.at(i,j);
            }
        }
        return *this;
    }

    /*
     * Square matrix specialization
     */
    template<typename T>
    class square_matrix : public matrix<T> {
    public:
        square_matrix(size_t N, T init = 0)
            : matrix<T>(N, N, init) {}

        square_matrix(std::initializer_list<std::initializer_list<T>> list) {
            if (list.size() == 0)
                throw std::out_of_range("(square_matrix::square_matrix) size zero");

            matrix<T>::table.reserve(list.size());
            
            for (const auto& entry : list) {
                if (entry.size() != list.size()) // square condition
                    throw std::out_of_range("(square_matrix::square_matrix) incoherent initializer list");

                matrix<T>::table.push_back(entry);
            }
        }

        inline size_t size() const {
            return matrix<T>::rows_nb();
        }
        
        /*
         * Generate the identity matrix
         */
        static square_matrix eye(size_t N) {
            square_matrix out(N);

            for (size_t i = 0; i < out.size(); ++i)
                out.at(i,i) = 1;

            return out;
        }
    };


    template <typename R, typename T>
    matrix<R> convert(const matrix<T>& A, const std::function<R (T)>& operation) {
        matrix<R> out(A.rows_nb(), A.cols_nb());

        for (size_t i = 0; i < A.rows_nb(); ++i) {
            for (size_t j = 0; j < A.cols_nb(); ++j) {
                out.at(i,j) = operation(A.at(i,j));
            }
        }

        return out;
    }
}

/*
 * std::abs overload
 */

namespace std {

    // element-wise absolute value
    template <typename T>
    inline qsim::math::matrix<double> abs(const qsim::math::matrix<T>& A) {
        return qsim::math::convert<double,T>(A, [&] (T value) { return std::abs(value); });
    }
}

/*
 * Operator overloading for matrix
 */

template<typename T, class Vector>
Vector operator*(const qsim::math::matrix<T>& A, const Vector& x) {
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

template<typename T, class Vector>
Vector operator<<(const qsim::math::matrix<T>& A, const Vector& x) {
    // TODO provvisor solution
    return A * x;
}

template<typename T, class Vector>
Vector operator*(const qsim::math::submatrix<T>& A, const Vector& x) {
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

/*
 * LU decomposition section
 */
namespace qsim::math {

    namespace helper {
        typedef std::deque<std::pair<size_t, size_t>> pivot_t;
        
        template<typename T, class Matrix>
        struct LU_output {
            square_matrix<T> L;
            square_matrix<T> U;

            // pivoting swap operations stack
            pivot_t P;

            LU_output(const Matrix& A)
                : L(square_matrix<T>::eye(A.size())), U(A) {}
        };

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
            auto maxind = helper::max( std::abs( matrix<T>(out.U.get_row(k, {k, N})) ));
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
                out.L(i,k) = out.U(i,k) / out.U(k,k);
                for (size_t j(k); j < N; ++j)
                    out.U(i,j) -= out.L(i,k) * out.U(k,j);
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
