#include "grid_wave.hpp"
#include "quantumsim/grid/wave.hpp"

using namespace godot;
using namespace qsim::grid;

#include "debug.hpp"

/*
 * Methods
 */
double grid_wave1D::real(int m) const {
    return (*const_wave())[m].real();
}

double grid_wave1D::imag(int m) const {
    return (*const_wave())[m].imag();
}

double grid_wave1D::prob(int m) const {
    return std::norm((*const_wave())[m]);
}

size_t grid_wave1D::N() const {
    return const_wave()->size();
}

void grid_wave1D::_set(int m, Vector2 complx) {
    wave_ref->at(m) = qsim::grid::wave_t(complx.x, complx.y);
}

Vector2 grid_wave1D::_get(int m) const {
    auto c = (*const_wave())[m];
    return Vector2(c.real(), c.imag());
}


/*
 * initializers
 */
void grid_wave1D::_init() {
    npdebug("Initializing grid_wave")
}
                                 
void grid_wave1D::_register_methods() {
    register_method("real", &grid_wave1D::real);
    register_method("imag", &grid_wave1D::imag);
    register_method("probability", &grid_wave1D::prob);
    register_method("N", &grid_wave1D::N);
    register_method("set", &grid_wave1D::_set);
    register_method("get", &grid_wave1D::_get);
}

