#include "wave_init1D.hpp"

using namespace godot;
using namespace qsim::grid;

wave_init1D::wave_init1D(size_t N)
    : qsystem1D::init_pack(std::bind(&wave_init1D::psi, this), N) {}

void wave_init1D::_init() {
    N = 0;
}

void wave_init1D::_register_methods() {
    register_property<wave_init1D, size_t>("N", &N, 0);
}

/*
 * Gauss init
 */

gauss_init1D::gauss_init1D(size_t _N
                           double _k0, double _r0,
                           double _sigma)
    : wave_init1D(_N), k0(_k0), r0(_r0), sigma(_sigma)
{
}


gauss_init1D* gauss_init1D::_new(size_t N, 
                                 double k0, 
                                 double r0, 
                                 double sigma) {
}

qsim::wave_t gauss_init1D::psi(double x) const {
    using namespace std::complex_literals;
    return exp(1i * k0 * x - pow((x - r0) / sigma, 2) / 2.0);
}

void gauss_init1D::_init() {
    k0 =  double(0.0, 0.0);   
    r0 =  double(0.0, 0.0);   
    sigma = Transform1D(double(1.0, 0.0), double(0.0, 1.0), double(0.0, 0.0));   
}

gauss_init1D * gauss_init1D::_new(size_t N, 
                                  size_t M, 
                                  double k0, 
                                  double r0, 
                                  Transform1D sigma) {

    gauss_init1D * out = gauss_init1D::_new();
    out->N = N;
    out->M = M;
    out->k0 = k0;
    out->r0 = r0;
    out->set_sigma(sigma);

    return out;
}


void gauss_init1D::set_sigma(const Tranform1D& sig) {
    sigma = sig.affine_inverse();
}

Transform1D gauss_init1D::get_sigma() const {
    return sigma;
}

void gauss_init1D::_register_methods() {
    register_property<gauss_init1D, double>("k0", &k0, double(0.0, 0.0));
    register_property<gauss_init1D, double>("r0", &r0, double(0.0, 0.0));
    register_property<gauss_init1D, Tranform1D>("sigma", &wave_init1D::set_sigma, &wave_init1D::get_sigma, Transform1D(double(1.0, 0.0), double(0.0, 1.0), double(0.0, 0.0)));
}

