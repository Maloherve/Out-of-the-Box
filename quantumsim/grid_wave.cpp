#include "grid/wave.hpp"
#include <algorithm>
#include <ostream>

using namespace qsim;

grid_wave& grid_wave::operator+=(const grid_wave& other) {
    
    const size_t N = std::min(size(), other.size()); 

    for (size_t k = 0; k < N; ++k)
        (*this)[k] += other[k];
    
    // push new values
    for (size_t k = N; other.size() > size(); ++k)
        push_back(other[k]);

    return *this;
}

grid_wave& grid_wave::operator-=(const grid_wave& other) {
    const size_t N = std::min(size(), other.size()); 

    for (size_t k = 0; k < N; ++k)
        (*this)[k] -= other[k];
    
    // push new values
    for (size_t k = N; other.size() > size(); ++k)
        push_back(-other[k]);

    return *this;
}

grid_wave& grid_wave::operator*=(const wave_t& z) {
    for (size_t k = 0; k < size(); ++k)
        (*this)[k] *= z;

    return *this;
}

grid_wave& grid_wave::operator/=(const wave_t& z) {
    for (size_t k = 0; k < size(); ++k)
        (*this)[k] /= z;

    return *this;
}

grid_wave operator+(grid_wave w, const grid_wave& other) {
    return w += other;
}

grid_wave operator-(grid_wave w, const grid_wave& other) {
    return w -= other;
}

grid_wave operator*(grid_wave w, const wave_t& z) {
    return w *= z;
}

grid_wave operator/(grid_wave w, const wave_t& z) {
    return w /= z;
}

grid_wave operator*(const wave_t& z, grid_wave w) {
    return w *= z;
}

grid_wave operator/(const wave_t& z, grid_wave w) {
    return w /= z;
}

std::ostream& operator<<(std::ostream& os, const grid_wave& v) {
    using namespace std;

    os << "(";
    for (size_t k = 0; k < v.size() - 1; ++k)
        os << v[k] << ", ";
    
    if (v.size() > 0)
        os << v.back();

    os << ")";
    return os;
}
