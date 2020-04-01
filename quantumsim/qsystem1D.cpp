#include "grid/qsystem1D.hpp"
#include "constants.hpp"
#include "potential.hpp"

using namespace qsim;

const math::diagonals<double, 3> qsystem1D::A = math::diagonals<double, 3>({math::sdiag_entry(-1, -1.0), math::sdiag_entry(0, 2.0), math::sdiag_entry(1, -1.0)});

qsystem1D::qsystem1D(double _m, 
                     const std::pair<double, double>& _bounds,
                     const grid_wave& _wave,
                     std::shared_ptr<potential<size_t>> _V,
                     std::shared_ptr<evo::integrator<size_t, grid_wave, H_obj>> _evolver
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

// TODO, implement trapezium integration
double qsystem1D::energy() const {
    return 0.0;
}

double qsystem1D::position() const {
    return 0.0;
}

double qsystem1D::momentum() const {
    return 0.0;
}
