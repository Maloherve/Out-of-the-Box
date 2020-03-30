#pragma once

#include <algorithm>

namespace qsim::math {
  
    /*
     * This class describes a square matrix composed by multiple
     * sub-diagonals (shifted diagonals), where
     * each diagonal admits only ONE value.
     * T = matrix field type
     * D = quantity of non-zero values in the first row
     * N = size of the matrix
     */ 
    template <typename T, size_t D, size_t N> 
    class diagonals {

    public:
        
        // admit std::sort
        struct entry : public std::pair<long int,T> {

            using std::pair<long int, T>::pair;

            bool operator<(const entry& other) const {
                // compare indices
                return first < other.first;    
            }

            bool operator<(long int index) const {
                // compare indices
                return first < index;    
            }
        };

    private:
        
        // long int = indices, 0 = diagonal
        // T = constant values at the first row
        std::array<entry,D> data;
        
        // look for a specific value via binary search O(log(D))
        auto find(long int index) {
            return std::lower_bound(data.begin(), data.end(), index);
        }

    public:
        diagonals(const std::array<entry,D>& values) 
            : data(values) {
                // sort by ascending index
                std::sort(data.begin(), data.end());
        }
        
        /*
         * row := current row index
         * action := functor (column index, column value) to be executed in the for loop
         */
        template <class F>
        void foreach(size_t row, F&& action) const {
            for (const std::pair<long int,T>& p : data) {
                // execute if in bounds
                if (p.first + static_cast<long int>(row) >= 0 &&
                    p.first + static_cast<long int>(row) < N)
                    action(static_cast<size_t>(p.first + row), p.second);
            }
        }

        /*
         * Ouput type
         */

        struct output_t : public std::pair<size_t,T> {
        
            using std::pair<size_t,T>::pair;

            inline size_t column() const {
                return this->first;
            }

            inline size_t value() const {
                return this->second;
            }
        }

        /*
         * Iterator to a row
         */

        struct const_iterator {
            size_t row;
            std::array<entry,D>::const_iterator it;

            const_iterator(size_t r, std::array<std::pair<long int,T>,D>::const_iterator _it)
                : row(r), it(_it) {
                // skip invalid addresses
                skip_invalid();
            }

            output_t operator*()() {
                return output_t(row + static_cast<size_t>(it->first), it->second);
            }

            output_t operator++(int) {
                auto out = *(*this);
                ++it;
                skip_invalid();
                return out;
            }

        private:

            inline void skip_invalid() {
                while ((*it).first + static_cast<long int>(row) >= 0 &&
                       (*it).first + static_cast<long int>(row) < N)
                    ++it;
            }
        };

        /*
         * Begin, end
         */

        const_iterator begin(size_t row) const {
            return const_iterator(row, data.begin());
        }

        const_iterator end(size_t row) const {
            return const_iterator(row, data.end());
        }

        /*
         * Row descriptor, best way to access a diagonals
         */

        struct row_array {

            size_t row_n;
            const diagonals<T,D,N>& reference;

            row_array(const diagonals<T,D,N>& ref, size_t row_number) : reference(ref), row_n(row_number) {}

            const_iterator begin() const {
                return reference.begin(row_n);
            }

            const_iterator end() const {
                return reference.end(row_n);
            }
        };

        /*
         * Row accessor and operator []
         * Advantage: a row_array is for auto loopable
         */

        inline row_array<T,D,N> get_row(size_t row) const {
            return row_array(*this, row);
        }

        inline row_array<T,D,N> operator[](size_t row) const {
            return row_array(*this, row);
        }
        
        // multiplication by scalar O(D)
        diagonals& operator*=(T a) {
            for (auto& p : data)
                p.second *= a;
        }
        
        // add a multiple of the identity matrix O(log(D))
        diagonals& operator+=(T a) {
            auto it = this->find(0);
            if (it != std::npos)
                it->second += a;
            else {
                // TODO, throw error
            }
        }
    };
}

template <typename T, size_t D, size_t N> 
const qsim::math::diagonals<T,D,N> operator*(qsim::math::diagonals<T,D,N> A, T a) {
    return A *= a;
}

template <typename T, size_t D, size_t N> 
const qsim::math::diagonals<T,D,N> operator*(T a, qsim::math::diagonals<T,D,N> A) {
    return A *= a;
}

template <typename T, size_t D, size_t N, template <typename _T, size_t _N> class V> 
V<T,N> operator*(const qsim::math::diagonals<T,D,N>& mat, const V<T,N>& v) {

    V<T,N> out;

    for (grid_t m = 0; m < v.size(); ++m) {
        out[m] = 0;
        for (auto&& row : mat[m]) // loop through the column
            out[m] += row.value() * v[row.column()];
    }
    
    return out;
}

