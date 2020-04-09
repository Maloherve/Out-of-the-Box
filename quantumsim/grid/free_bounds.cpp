#include "grid/free_bounds.hpp"

using namespace qsim::grid;

free_bounds::free_bounds(double low, double up, double low_damp, double up_damp, const wave_vector& psi) 
    : qsim::interval(low, up), 
      lower_damping(low_damp), lower_damping(low_damp), wave(psi) {}

std::pair<wave_t, wave_t> free_bounds::continuity(const wave_vector& wave) const {
    // free boundary conditions
    return {2.0 * wave[1u] - wave[2u], 2.0 * (*(wave.end()-2)) - (*(wave.end()-3))}
}

wave_t free_bounds::before()(double x) const {
    return wave.front() * exp(damping * (x - lower));
}

wave_t free_bounds::after()(double x) const {
    return wave.back() * exp(- damping * (x - upper));
}

double free_bounds::normalization() const {
    return wave.front() / (2*lower_damping) + wave.back() / (2*upper_damping);
}
