#include "wave_init2D.hpp"

using namespace godot;
using namespace qsim::grid;

wave_init2D::wave_init2D(size_t N, size_t M)
    : qsystem2D::init_pack(std::bind(&wave_init2D::psi, this), N, M) {}

void wave_init2D::_init() {
    N = 0;
    M = 0;
}

void wave_init2D::_register_methods() {
    register_property<wave_init2D, size_t>("N", &N, 0);
    register_property<wave_init2D, size_t>("M", &M, 0);
}

/*
 * Gauss init
 */

gauss_init2D::gauss_init2D(size_t N,
                           size_t M,
                           Vector2 _k0, Vector2 _r0,
                           Vector2 _sigma)
    : wave_init2D(N, M), k0(_k0), r0(_r0), sigma(_sigma)
{
}


gauss_init2D* gauss_init2D::_new(size_t N, 
                                 size_t M, 
                                 Vector2 k0, 
                                 Vector2 r0, 
                                 Transform2 sigma) {
}

qsim::wave_t gauss_init2D::psi(double x, double y) const {
    using namespace std::complex_literals;
    Vector2 r(x - r0.x, y - r0.y);
    double pw = r.x * sigma.x.x * r.x + r.y * sigma.x.y * r.x +
                r.x * sigma.x.y * r.y + r.y * sigma.y.y * r.y;
    return exp(1i * (k0.x * x + k0.y * y) - pw / 2.0);
}

void gauss_init2D::_init() {
    k0 =  Vector2(0.0, 0.0);   
    r0 =  Vector2(0.0, 0.0);   
    sigma = Transform2D(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);   
}

gauss_init2D * gauss_init2D::_new(size_t N, 
                                  size_t M, 
                                  Vector2 k0, 
                                  Vector2 r0, 
                                  Transform2D sigma) {

    gauss_init2D * out = gauss_init2D::_new();
    out->N = N;
    out->M = M;
    out->k0 = k0;
    out->r0 = r0;
    out->set_sigma(sigma);

    return out;
}


void gauss_init2D::set_sigma(const Tranform2D& sig) {
    sigma = sig.affine_inverse();
}

Transform2D gauss_init2D::get_sigma() const {
    return sigma;
}

void gauss_init2D::_register_methods() {
    register_property<gauss_init2D, Vector2>("k0", &k0, Vector2(0.0, 0.0));
    register_property<gauss_init2D, Vector2>("r0", &r0, Vector2(0.0, 0.0));
    register_property<gauss_init2D, Tranform2D>("sigma", &wave_init2D::set_sigma, &wave_init2D::get_sigma, Transform2D(Vector2(1.0, 0.0), Vector2(0.0, 1.0), Vector2(0.0, 0.0)));
}

