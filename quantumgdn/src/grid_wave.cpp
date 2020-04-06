#include "grid_wave.hpp"
#include "quantumsim/grid/wave.hpp"

using namespace godot;
using namespace qsim::grid;

grid_wave::grid_wave(wave_vector * wave, bool free) 
    : wave_ref(wave), to_be_deleted(free) {}


grid_wave::~grid_wave() {
    if (to_be_deleted && wave_ref != nullptr)
        delete wave_ref;
}

void grid_wave::set_instance(wave_vector * wave, bool free) {
    wave_ref = wave;
    to_be_deleted = free;
}

/*
 * Methods
 */
double grid_wave::real(int m) const {
    return (*const_wave())[m].real();
}

double grid_wave::imag(int m) const {
    return (*const_wave())[m].imag();
}

double grid_wave::prob(int m) const {
    return std::norm((*const_wave())[m]);
}

size_t grid_wave::_size() const {
    return const_wave()->size();
}

void grid_wave::_set(size_t m, Vector2 complx) {
    (*const_wave())[m] = qsim::grid::wave_t(complx.x, complx.y);
}

Vector2 grid_wave::_get(int m) const {
    auto c = (*const_wave())[m];
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
