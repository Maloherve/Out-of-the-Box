#include "wave_packets.hpp"
#include "grid_wave.hpp"
#include "quantumsim/grid/wave.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

#include "debug.hpp"

#include <Ref.hpp>

using namespace godot;
using namespace qsim::grid;

/*
 * General wave_packet definition
 */

wave_packet::wave_packet()
        : op([&](size_t k) { return wave_t(k,0); }), N(0) {}

wave_packet::~wave_packet() {
    npdebug("Destroying packet")
}

wave_packet::Op_iter wave_packet::begin() const {
    return Op_iter(op, 0);
}

wave_packet::Op_iter wave_packet::end() const {
    return Op_iter(op, N);
}

void wave_packet::_init() {
    N = 50;
}

void wave_packet::_register_methods() {
    register_property<wave_packet, size_t>("size", &wave_packet::N, 50); 
}

/*
 * wave_packet iterator
 */

wave_packet::Op_iter::Op_iter(const wave_packet::Op& _op, size_t _k) : op(_op), k(_k) {}

wave_t wave_packet::Op_iter::operator*() const {
    return op(k);
}

wave_packet::Op_iter::~Op_iter() {}

bool wave_packet::Op_iter::operator!=(const wave_packet::Op_iter& other) const {
    return k != other.k;
}

wave_packet::Op_iter& wave_packet::Op_iter::operator++() {
    ++k;
    return (*this);
}

/*
 * Gaussian definition
 */

gaussian_packet::gaussian_packet(size_t _N, double _first, double _second, double n, double _x0, double _sigma) : 
            wave_packet([&] (size_t k) { return (*this)(k); }, _N), 
            first(_first), 
            L(_second - _first), 
            k0(2 * M_PI * n / L), x0(_x0), 
            sigma(_sigma) {}

gaussian_packet::~gaussian_packet() {
    npdebug("Calling")
}

wave_t gaussian_packet::operator()(size_t k) const {

    using namespace std::complex_literals;
    double x = first + double(k * L) / N;
    return exp(1i * k0 * x - pow((x - x0)/sigma, 2) / 2);                        
}

void gaussian_packet::_init() {
    first = -1.0;
    L = 2.0;
    k0 = 0.0;
    x0 = 0.0;
    sigma = 1.0;
}

void gaussian_packet::set_second(double value) {
    L = value - first;
}

double gaussian_packet::get_second() const {
    return first + L;
}

void gaussian_packet::set_n(double n) {
    k0 = 2 * M_PI * n / L;
}

double gaussian_packet::get_n() const {
    return k0 * L / (2 * M_PI);
}

void gaussian_packet::_register_methods() {
    register_property<gaussian_packet, double>("first", &gaussian_packet::first, -1.0);
    register_property<gaussian_packet, double>("second", &gaussian_packet::set_second, &gaussian_packet::get_second, 1.0);
    register_property<gaussian_packet, double>("n", &gaussian_packet::set_n, &gaussian_packet::get_n, 0.0);
    register_property<gaussian_packet, double>("x0", &gaussian_packet::x0, 0.0);
    register_property<gaussian_packet, double>("sigma", &gaussian_packet::sigma, 1.0);
}
