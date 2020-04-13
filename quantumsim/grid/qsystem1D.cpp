#include "grid/qsystem1D.hpp"
#include "constants.hpp"
#include "potential.hpp"

#include "grid/integrators.hpp"

#include "debug.hpp"

using namespace qsim::grid;

const qsim::math::diagonals<double, 3> qsystem1D::A = math::diagonals<double, 3>({math::sdiag_entry(-1, 1.0), math::sdiag_entry(0, -2.0), math::sdiag_entry(1, 1.0)});

qsystem1D::qsystem1D(double _m, 
                     double _dx,
                     std::shared_ptr<potential<size_t>> _V,
                     const init_pack& init,
                     std::shared_ptr<evolver<size_t, wave_vector, grid_H_1D>> _evolver,
                     double hbar
                     ) : qgridsystem<H_matrix_1D>(_m, init.generate(_dx), _V, _evolver, hbar),
                        // compose the hemiltonian matrix
                        H(
                            0.0,
                            H_zero(), 
                            std::function<double (size_t)>([&] (size_t k) -> double { return V()(k); }) 
                         ), 
                        dx(_dx)
    {
        normalize();
    }


wave_vector qsystem1D::init_pack::generate(double dx) const {
    wave_vector w(N, qsim::wave_t(0.0));

    // construct it using the analytic expression
    for (size_t i = 0; i < N; ++i) {
        w[i] = f(i * dx);
    }

    return w;
}

qsim::math::diagonals<double, 3> qsystem1D::H_zero() const {
    return math::diagonals<double,3>((-pow(hbar()/dx, 2) / (2 * mass())) * A);
}

void qsystem1D::update_H() {
    /*
     * Regenerate the part of the matrix which corresponds to 
     * the H_0 hamiltonian
     */
    H.get<1>() = H_zero();
}

void qsystem1D::set_hbar(double _plank) {
     //npdebug("Setting hbar: ", _plank)
     qgridsystem<H_matrix_1D>::set_hbar(_plank);
     update_H();
}

void qsystem1D::set_mass(double _m) {
    qgridsystem<H_matrix_1D>::set_mass(_m);
    update_H();
}

double qsystem1D::norm() const {
    double a(0);

    for (size_t i = 0; i < wave.size(); ++i)
        a += std::norm(wave[i]);

    a *= dx;

    return a;
}

void qsystem1D::replace_wave(const init_pack& init) {
    wave = init.generate(dx);
    normalize();
}

void qsystem1D::set_delta(double _dx) {
    dx = _dx;
    update_H();
}

double qsystem1D::delta() const {
    return dx;
}

double qsystem1D::energy() const {
    const double H0 = -hbar() * hbar() / (2*mass());
    wave_t result = grid::grid_integrate(wave, [&] (const neighbourhood<>& map, size_t location) {
                    return H0 * (map.at(location, -1) + map.at(location, 1))  
                        + (V()(location) - 2 * H0) * map.at(location, 0); // return the value itself
                }, dx);

    if (abs(result.imag()) > qsim::machine_prec)
        throw result; // TODO, a real error

    return result.real();
}

double qsystem1D::position() const {
    double pos(0);

    for (size_t i = 0; i < wave.size(); ++i)
        pos += x(i) * std::norm(wave[i]);

    pos *= dx;

    return pos;
}

double qsystem1D::momentum() const {
    wave_t mom = grid::grid_integrate(wave, [&] (const neighbourhood<>& map, size_t location) {
                    return (map.at(location, 1) - map.at(location, -1)) / (2 * dx); // return the value itself
                }, dx);

    if (abs(mom.imag()) > qsim::machine_prec)
        throw mom; // TODO, a real error

    return mom.real();
}

wave_vector::iterator qsystem1D::begin() {
    return wave.begin();
}

wave_vector::iterator qsystem1D::end() {
    return wave.end();
}

wave_vector::const_iterator qsystem1D::begin() const {
    return wave.begin();
}

wave_vector::const_iterator qsystem1D::end() const {
    return wave.end();
}
