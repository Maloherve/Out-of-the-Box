#include "grid_wave.hpp"
#include "quantumsim/grid/wave.hpp"

using namespace godot;

grid_wave::grid_wave() : wave_vector() {}
grid_wave::~grid_wave() {}

/*
 * Methods
 */
double grid_wave::real(int m) const {
    return (*this)[m].real();
}

double grid_wave::imag(int m) const {
    return (*this)[m].imag();
}

double grid_wave::prob(int m) const {
    return std::norm((*this)[m]);
}

size_t grid_wave::_size() const {
    return size();
}

void grid_wave::_set(size_t m, Vector2 complx) {
    (*this)[m] = qsim::grid::wave_t(complx.x, complx.y);
}

Vector2 grid_wave::_get(int m) const {
    auto c = (*this)[m];
    return Vector2(c.real(), c.imag());
}

/*void grid_wave::_push(const Array& list) {
    push(list);
}*/

/*
 * initializers
 */
void grid_wave::_init() {}
                                 
void grid_wave::_register_methods() {
    register_method("real", &grid_wave::real);
    register_method("imag", &grid_wave::imag);
    register_method("prob", &grid_wave::prob);
    register_method("size", &grid_wave::_size);
    register_method("set", &grid_wave::_set);
    register_method("get", &grid_wave::_get);
    //register_method("push", &grid_wave::_push);
}
