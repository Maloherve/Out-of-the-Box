#include "grid/wave.hpp"
#include <algorithm>
#include <ostream>

using namespace qsim::grid;

wave_vector& wave_vector::operator+=(const wave_vector& other) {
    
    const size_t N = std::min(size(), other.size()); 

    for (size_t k = 0; k < N; ++k)
        (*this)[k] += other[k];
    
    // push new values
    for (size_t k = N; other.size() > size(); ++k)
        push_back(other[k]);

    return *this;
}

wave_vector& wave_vector::operator-=(const wave_vector& other) {
    const size_t N = std::min(size(), other.size()); 

    for (size_t k = 0; k < N; ++k)
        (*this)[k] -= other[k];
    
    // push new values
    for (size_t k = N; other.size() > size(); ++k)
        push_back(-other[k]);

    return *this;
}

wave_vector& wave_vector::operator*=(const wave_t& z) {
    for (size_t k = 0; k < size(); ++k)
        (*this)[k] *= z;

    return *this;
}

wave_vector& wave_vector::operator/=(const wave_t& z) {
    for (size_t k = 0; k < size(); ++k)
        (*this)[k] /= z;

    return *this;
}

wave_vector operator+(wave_vector w, const wave_vector& other) {
    return w += other;
}

wave_vector operator-(wave_vector w, const wave_vector& other) {
    return w -= other;
}

wave_vector operator*(wave_vector w, const wave_t& z) {
    return w *= z;
}

wave_vector operator/(wave_vector w, const wave_t& z) {
    return w /= z;
}

wave_vector operator*(const wave_t& z, wave_vector w) {
    return w *= z;
}

wave_vector operator/(const wave_t& z, wave_vector w) {
    return w /= z;
}

std::ostream& operator<<(std::ostream& os, const wave_vector& v) {
    using namespace std;

    os << "(";
    for (size_t k = 0; k < v.size() - 1; ++k)
        os << v[k] << ", ";
    
    if (v.size() > 0)
        os << v.back();

    os << ")";
    return os;
}
