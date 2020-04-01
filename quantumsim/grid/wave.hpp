#pragma once

#include <complex>
#include <vector>
#include <iosfwd>

namespace qsim {

    typedef std::complex<double> wave_t;
    /*
    * Discretized wave function
    */
    class grid_wave : private std::vector<wave_t> {
        
    public:
        
        // inherit constructors
        using std::vector<wave_t>::vector;

        // inherit other methods
        using std::vector<wave_t>::size;
        using std::vector<wave_t>::begin;
        using std::vector<wave_t>::end;
        using std::vector<wave_t>::front;
        using std::vector<wave_t>::back;
        using std::vector<wave_t>::operator=;
        
        // define an out-of-bounds different behaviour for the const case
        inline wave_t operator[](int i) const {
            return (i < 0 || static_cast<size_t>(i) >= size()) ? 
                0 : std::vector<wave_t>::operator[](static_cast<size_t>(i));
        }

        inline wave_t operator[](size_t i) const {
            return (i >= size()) ? 
                0 : std::vector<wave_t>::operator[](i);
        }

        inline wave_t& operator[](size_t i) {
            return std::vector<wave_t>::operator[](i);
        }

        grid_wave& operator+=(const grid_wave&);

        grid_wave& operator-=(const grid_wave&);
        grid_wave& operator*=(const wave_t&);
        grid_wave& operator/=(const wave_t&);
    };
}

qsim::grid_wave operator+(qsim::grid_wave, const qsim::grid_wave&);
qsim::grid_wave operator-(qsim::grid_wave, const qsim::grid_wave&);

qsim::grid_wave operator*(qsim::grid_wave, const qsim::wave_t&);
qsim::grid_wave operator/(qsim::grid_wave, const qsim::wave_t&);
qsim::grid_wave operator*(const qsim::wave_t&, qsim::grid_wave);
qsim::grid_wave operator/(const qsim::wave_t&, qsim::grid_wave);

std::ostream& operator<<(std::ostream&, const qsim::grid_wave&);

