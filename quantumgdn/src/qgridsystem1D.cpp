#include "qgridsystem1D.hpp"

#include "grid_wave.hpp"

#include "quantumsim/evolvers/crank_nicholson.hpp"
#include "curve_potential.hpp"

#include "debug.hpp"

using namespace godot;
using namespace qsim::grid;

qgridsystem1D::qgridsystem1D() 
    : 
        gdqsystem(std::make_shared<godot::curve_potential>(this)),
        qsystem1D(1.0, 1.0, gdqsystem::potential(), 
                gauss_init1D(),
                std::make_shared<qsim::evo::crank_nicholson>(), 1.0)
{
}

void qgridsystem1D::_init() {
    _wave = grid_wave1D::_new();
    _wave->wave_ref = &wave;
}

qgridsystem1D::~qgridsystem1D() {
    npdebug("Destructing 1D system")
    delete _wave;
}

void qgridsystem1D::_register_methods() {
    npdebug("Registering methods")
    // godot built-in
    register_method("_physics_process", &qgridsystem1D::_fixed_process);
    register_method("_ready", &qgridsystem1D::_ready);

    // direct methods
    register_method("energy", &qgridsystem1D::_energy);
    register_method("mean_position", &qgridsystem1D::_position);
    register_method("mean_momentum", &qgridsystem1D::_momentum);
    register_method("set_wave", &qgridsystem1D::set_wave);
    register_method("wave", &qgridsystem1D::get_wave);
    register_method("psi", &qgridsystem1D::psi);
    register_method("psi_norm", &qgridsystem1D::psi_norm);
    register_method("psi_real", &qgridsystem1D::psi_real);
    register_method("psi_imag", &qgridsystem1D::psi_imag);
    register_method("N", &qgridsystem1D::N);
    register_method("x", &qgridsystem1D::x);
    register_method("potential_at", &qgridsystem1D::potential_at);
    register_method("potential_at_index", &qgridsystem1D::potential_at_index);
    
    // properties
    register_property<qgridsystem1D, double>("mass", &qgridsystem1D::set_mass, &qgridsystem1D::mass, 1.0);
    register_property<qgridsystem1D, double>("hbar", &qgridsystem1D::set_hbar, &qgridsystem1D::get_hbar, 1.0);
}


double qgridsystem1D::_energy() const {
    return qsystem1D::energy();
}

double qgridsystem1D::_position() const {
    return qsystem1D::position() + get_position().x;
}

double qgridsystem1D::_momentum() const {
    return qsystem1D::momentum();
}

void qgridsystem1D::set_hbar(double _plank) {
    qsystem1D::set_hbar(_plank);
}

double qgridsystem1D::get_hbar() const {
    return qsystem1D::hbar();
}

Vector2 qgridsystem1D::psi(int index) const {
    return Vector2(wave[index].real(), wave[index].imag());
}

double qgridsystem1D::psi_norm(int index) const {
    return std::norm(wave[index]);
}

double qgridsystem1D::psi_real(int index) const {
    return wave[index].real();
}

double qgridsystem1D::psi_imag(int index) const {
    return wave[index].imag();
}

double qgridsystem1D::potential_at(double x) const {
    return potential()->at(Vector2(x,0));
}

double qgridsystem1D::potential_at_index(size_t k) const {
    return (*potential())(k);
}

size_t qgridsystem1D::N() const {
    return size();
}

double qgridsystem1D::x(size_t i) const {
    return qsystem1D::x(i) + get_position().x;
}

void qgridsystem1D::set_wave(Ref<wave_init1D> init) {
    if (init != nullptr) {

        npdebug("Setting wave: width = ", get_width(), ", N = ", init->N)
        set_delta(get_width() / static_cast<double>(init->N));

        replace_wave(*(*init));
        _wave->wave_ref = &wave;
    } else {
        npdebug("Setting a null initialization packet")
    }

}

grid_wave1D * qgridsystem1D::get_wave() const {
    return _wave;
}

Vector2 qgridsystem1D::location(size_t k) const {
    return Vector2(x(k), 0);
}


void qgridsystem1D::_fixed_process(double dt) {
    // default values
    qsim::grid::qsystem1D::evolve(dt);
    qsim::grid::qsystem1D::normalize();
}

void qgridsystem1D::_ready() {
    gdqsystem::_ready();
}
