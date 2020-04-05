#include "grid_wave.hpp"
#include "quantumsim/grid/wave.hpp"

using namespace godot;

grid_wave::grid_wave() : m_wave(nullptr) {}
grid_wave::~grid_wave() {}

#define const_index(m) (*const_cast<const qsim::grid::wave_vector*>(m_wave))[m]

/*
 * Methods
 */
double grid_wave::real(int m) const {
    return const_index(m).real();
}

double grid_wave::imag(int m) const {
    return const_index(m).imag();
}

double grid_wave::prob(int m) const {
    return std::norm(const_index(m));
}

size_t grid_wave::size() const {
    return m_wave->size();
}

void grid_wave::set(size_t m, Vector2 complx) {
    (*m_wave)[m] = qsim::grid::wave_t(complx.x, complx.y);
}

Vector2 grid_wave::get(int m) const {
    auto c = const_index(m);
    return Vector2(c.real(), c.imag());
}

/*
 * initializers
 */
void grid_wave::_init() {}
                                 
void grid_wave::_register_methods() {
    register_method("real", &grid_wave::real);
    register_method("imag", &grid_wave::imag);
    register_method("prob", &grid_wave::prob);
    register_method("size", &grid_wave::size);
    register_method("set", &grid_wave::set);
    register_method("get", &grid_wave::get);
}
