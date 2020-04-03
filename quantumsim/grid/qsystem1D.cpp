#include "grid/qsystem1D.hpp"
#include "constants.hpp"
#include "potential.hpp"

    #include "grid/integrators.hpp"

using namespace qsim::grid;

const qsim::math::diagonals<double, 3> qsystem1D::A = math::diagonals<double, 3>({math::sdiag_entry(-1, 1.0), math::sdiag_entry(0, -2.0), math::sdiag_entry(1, 1.0)});

qsystem1D::qsystem1D(double _m, 
                     const std::pair<double, double>& _bounds,
                     const wave_vector& _wave,
                     std::shared_ptr<potential<size_t>> _V,
                     std::shared_ptr<evolver<size_t, wave_vector, grid_H_1D>> _evolver
                     ) : qgridsystem<H_matrix_1D>(_m, _wave, _V, _evolver),
                        boundaries(_bounds),
                        // TODO, adjust correct dx
                        dx((_bounds.second - _bounds.first) / _wave.size()),
                        // compose the hemiltonian matrix
                        H(
                            0.0,
                            math::diagonals<double,3>((- hbar * hbar / (2 * mass() * dx * dx)) * A), 
                            std::function<double (size_t)>([&] (size_t k) -> double { return V()(k); }) 
                         )
        {
        }


void qsystem1D::set_mass(double _m) {
    double old = mass();
    qgridsystem<H_matrix_1D>::set_mass(_m);
    // using mass assures the positivity
    // get<1> refers to the A matrix component
    H.get<1>() *= mass() / old;
}

double qsystem1D::normalize() {

    wave_t norm = grid::grid_integrate(psi(), [&] (const neighbourhood<>& map, size_t location) {
                    return map.at(location, 0); // return the value itself
                }, dx);

    if (abs(norm.imag()) > qsim::machine_prec)
        throw norm; // TODO, a real error

    double out = sqrt(norm.real());
    
    psi() /= out;  
    return out;
}

std::vector<double> qsystem1D::generate_map() const {

    std::vector<double> out;
    out.reserve(psi().size());

    for (size_t k = 0; k < psi().size(); ++k)
        out.push_back(boundaries.first + dx * k);

    return out;
}

// TODO, implement trapezium integration
double qsystem1D::energy() const {
    const double H0 = -hbar * hbar / (2*mass());
    wave_t result = grid::grid_integrate(psi(), [&] (const neighbourhood<>& map, size_t location) {
                    return H0 * (map.at(location, -1) + map.at(location, 1))  
                        + (V()(location) - 2 * H0) * map.at(location, 0); // return the value itself
                }, dx);

    if (abs(result.imag()) > qsim::machine_prec)
        throw result; // TODO, a real error

    return result.real();
}

double qsystem1D::position() const {
    return 0.0;
}

double qsystem1D::momentum() const {
    return 0.0;
}
