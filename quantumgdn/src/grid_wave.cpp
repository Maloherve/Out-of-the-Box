#include "grid_wave.hpp"
#include "quantumsim/grid/wave.hpp"
#include "wave_packets.hpp"

using namespace godot;
using namespace qsim::grid;

#include "debug.hpp"

grid_wave::grid_wave(wave_vector * wave) 
    : wave_ref(wave) {}


grid_wave::~grid_wave() {
    npdebug("Successfully deleted");
}

void grid_wave::set_instance(wave_vector * wave) {
    npdebug("Setting instance")
    wave_ref = wave;
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
    (*wave_ref)[m] = qsim::grid::wave_t(complx.x, complx.y);
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
void grid_wave::_init() {
    npdebug("Initializing grid_wave")
}
                                 
void grid_wave::_register_methods() {
    register_method("real", &grid_wave::real);
    register_method("imag", &grid_wave::imag);
    register_method("prob", &grid_wave::prob);
    register_method("size", &grid_wave::_size);
    register_method("set", &grid_wave::_set);
    register_method("get", &grid_wave::_get);
    //register_method("set_packet", &grid_wave::_set_packet);
}

void grid_wave::move_into(qsim::grid::wave_vector& other) {
    other = std::move(*wave_ref);
    wave_ref = &other; // other must persist
}

/*void grid_wave::_set_packet(Ref<wave_packet> pack) {
    if (wave_ref == nullptr) {
        npdebug("Null wave")
    } else if (pack == nullptr) {
        npdebug("Null pack")
    } else {
        npdebug("Setting packet")
        wave_ref->clear();
        wave_ref->push(*(*pack));
    }
}*/
