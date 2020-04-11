#include "wave_init2D.hpp"

using namespace godot;
using namespace qsim::grid;

wave_init2D::wave_init2D(const std::function<qsim::wave_t (double, double)>& f)
    : qsystem2D::init_pack(f) {}


qsim::wave_t wave_init2D::operator*(double x, double y) const {
    return f(x,y);
}

void wave_init2D::_init() {
    N = 0;
    M = 0;
}

static void wave_init2D::_register_methods() {
    register_property<wave_init2D, size_t>("N", &N, 0);
    register_property<wave_init2D, size_t>("M", &M, 0);
}

/*
 * Gauss init
 */

gauss_init2D::gauss_init2D(size_t N, size_t M,
                           Vector2 k0, Vector2 r0,
                           Vector2 sigma)
    : init_pack([&] (double x, double y) {
        using namespace std::complex_literals;
        return exp(1i * k0.x * x - pow((x - r0.x)/sigma.x, 2) / 2)
             * exp(1i * k0.y * y - pow((y - r0.y)/sigma.y, 2) / 2);
    }, N, M) 
{
}



