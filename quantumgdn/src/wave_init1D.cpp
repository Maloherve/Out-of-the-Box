#include "wave_init1D.hpp"

using namespace godot;
using namespace qsim::grid;

using namespace std::placeholders;

wave_init1D::wave_init1D(int N)
    : qsystem1D::init_pack(std::bind(&wave_init1D::psi, this, _1), size_t(N)) {}

void wave_init1D::_init() {
    N = 0;
}

void wave_init1D::set_N(int N) {
    if (N >= 0)
        this->N = N;
}

int wave_init1D::get_N() const {
    return static_cast<int>(N);
}

void wave_init1D::_register_methods() {
    register_property<wave_init1D, int>("N", &wave_init1D::set_N, &wave_init1D::get_N, 0);
}

/*
 * Gauss init
 */

gauss_init1D::gauss_init1D(int _N,
                           double _k0, double _r0,
                           double _sigma)
    : wave_init1D(_N), k0(_k0), r0(_r0), sigma(_sigma)
{
}


gauss_init1D* gauss_init1D::_new(int N, 
                                 double k0, 
                                 double r0, 
                                 double sigma) {

    gauss_init1D * init = gauss_init1D::_new();

    init->N = N;
    init->k0 = k0;
    init->r0 = r0;
    init->sigma = sigma;

    return init;
}

qsim::wave_t gauss_init1D::psi(double x) const {
    using namespace std::complex_literals;
    return exp(1i * k0 * x - pow((x - r0) / sigma, 2) / 2.0);
}

void gauss_init1D::_init() {
    k0 = 0.0;   
    r0 = 0.0;   
    sigma = 1.0;   
}


void gauss_init1D::set_sigma(double sig) {
    sigma = sig;
}

double gauss_init1D::get_sigma() const {
    return sigma;
}

void gauss_init1D::_register_methods() {
    register_property<gauss_init1D, double>("k0", &gauss_init1D::k0, 0.0);
    register_property<gauss_init1D, double>("r0", &gauss_init1D::r0, 0.0);
    register_property<gauss_init1D, double>("sigma", &gauss_init1D::set_sigma, &gauss_init1D::get_sigma, 1.0);
}

