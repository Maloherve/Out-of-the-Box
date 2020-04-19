#pragma once

#include <vector>
#include <limits>
#include <deque>
#include <functional>

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
        submatrix(matrix<T>* _ref, std::pair<size_t, size_t> _rows, std::pair<size_t, size_t> _cols);
        
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
        submatrix& operator=(const submatrix&);
        submatrix& operator=(submatrix&&) = delete;

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
        void swap(submatrix& other);        
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

            inline const T& _at(size_t j) const {
                return _at(this->rows.first, j);
            }

            inline T& _at(size_t j) {
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

        matrix(size_t N, size_t M, T init = 0);

        matrix(std::initializer_list<std::initializer_list<T>> list);
        
        matrix(const matrix&) = default;

        // move table
        matrix(matrix&&) = default;
        matrix& operator=(matrix&&) = default;
        matrix& operator=(const matrix&) = default;

        // construct copying a submatrix
        matrix(const submatrix<T>& sub);

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
        submatrix<T> restrict(std::pair<size_t,size_t> rows = all, std::pair<size_t, size_t> cols = all); 

        /*
         * Create a sub-matrix with a column indexing restriction
         */
        row_vector<T> get_row(size_t i, std::pair<size_t, size_t> cols);
        
        /*
         * Row descriptor without a column restriction
         */
        inline row& get_row(size_t i) {
            return table[i];
        }

        inline const row& get_row(size_t i) const {
            return table[i];
        }

        /*
         * Create a sub-matrix with a row indexing restriction
         */
        column_vector<T> get_column(size_t j, std::pair<size_t,size_t> rows = all);

        /*
         * swap the matrix internally
         */

        void swap_rows(std::pair<size_t, size_t> swp);
        
        // using a column range
        void swap_rows(std::pair<size_t, size_t> swp, std::pair<size_t, size_t> col_range);

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
     * Square matrix specialization
     */
    template<typename T>
    class square_matrix : public matrix<T> {
    public:
        square_matrix(size_t N, T init = 0);

        square_matrix(std::initializer_list<std::initializer_list<T>> list);

        inline size_t size() const {
            return matrix<T>::rows_nb();
        }
        
        /*
         * Generate the identity matrix
         */
        static square_matrix eye(size_t N);
    };

    template <typename R, typename T>
    matrix<R> convert(const matrix<T>& A, const std::function<R (T)>& operation);
}

/*
 * std::abs overload
 */

namespace std {

    // element-wise absolute value
    template <typename T>
    qsim::math::matrix<double> abs(const qsim::math::matrix<T>& A);
}

/*
 * Operator overloading for matrix
 */

template<typename T, class Vector>
Vector operator*(const qsim::math::matrix<T>& A, const Vector& x);

template<typename T, class Vector>
Vector operator*(const qsim::math::submatrix<T>& A, const Vector& x);

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
        std::pair<size_t, size_t> max(const qsim::math::matrix<T>& mat);
        
        // gauss elimination of lower triangular
        template<typename T, class Vector>
        Vector solve_lower(const square_matrix<T> L, Vector y);

        // gauss elimination of upper triangular
        template<typename T, class Vector>
        Vector solve_upper(const square_matrix<T> U, Vector x);
    }
    
    /*
     * Compute LU decomposition
     * A = intering square matrix
     * Requirements: size(), static_cast to square_matrix<T>
     */
    template<typename T, class Matrix>
    helper::LU_output<T, Matrix> LU_decomposition(const Matrix& A);
       
    /*
     * Solve A * s = b system
     */ 
    template<typename T, class Matrix, class Vector>
    Vector solve(const Matrix& A, Vector b);
}

// implementation
#include "matrix_template.hpp"

