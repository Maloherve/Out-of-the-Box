#pragma once

#include <complex>
#include <vector>
#include <iosfwd>
#include "fw.hpp"

namespace qsim::grid {

    /*
    * Discretized wave function
    */
    class wave_vector : private std::vector<wave_t> {
        
    public:
        
        // inherit constructors
        using std::vector<wave_t>::vector;

        // inherit other methods
        using std::vector<wave_t>::size;
        using std::vector<wave_t>::clear;
        using std::vector<wave_t>::begin;
        using std::vector<wave_t>::end;
        using std::vector<wave_t>::front;
        using std::vector<wave_t>::back;
        using std::vector<wave_t>::operator=;

        using std::vector<wave_t>::push_back;
        using std::vector<wave_t>::insert;
        
        // define an out-of-bounds different behaviour for the const case
        inline wave_t operator[](int i) const {
            return (i < 0 || static_cast<size_t>(i) >= size()) ? 
                0 : std::vector<wave_t>::operator[](static_cast<size_t>(i));
        }

        inline const wave_t& operator[](size_t i) const {
            return std::vector<wave_t>::operator[](i);
        }

        inline wave_t& operator[](size_t i) {
            return std::vector<wave_t>::operator[](i);
        }

        wave_vector& operator+=(const wave_vector&);

        wave_vector& operator-=(const wave_vector&);
        wave_vector& operator*=(const wave_t&);
        wave_vector& operator/=(const wave_t&);
        
        /*
         * mean over an operation A, <psi|A|psi>
         */
        template<class Op, class Integrator>
        wave_t bra_ket(const Op& A, Integrator&& integr) const {

            return integr(*this, A);
        }

        /*
         * Add precompiled iterable object
         */

        template<class Iter>
        void push(const Iter& iter) {
            for (const auto& val : iter)
                this->push_back(val);
        }

        template<class Iterator>
        void push(Iterator begin, const Iterator end) {
            for (;begin != end; ++begin)
                this->push_back(*begin);
        }
    };
}

qsim::grid::wave_vector operator+(qsim::grid::wave_vector, const qsim::grid::wave_vector&);
qsim::grid::wave_vector operator-(qsim::grid::wave_vector, const qsim::grid::wave_vector&);

qsim::grid::wave_vector operator*(qsim::grid::wave_vector, const qsim::grid::wave_t&);
qsim::grid::wave_vector operator/(qsim::grid::wave_vector, const qsim::grid::wave_t&);
qsim::grid::wave_vector operator*(const qsim::grid::wave_t&, qsim::grid::wave_vector);
qsim::grid::wave_vector operator/(const qsim::grid::wave_t&, qsim::grid::wave_vector);

std::ostream& operator<<(std::ostream&, const qsim::grid::wave_vector&);

