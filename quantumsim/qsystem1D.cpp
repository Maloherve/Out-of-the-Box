#include "grid/qsystem1D.hpp"

using namespace qsim;

qsystem1D::qsystem1D(double _m, 
                     const std::pair<double, double>& _bounds,
                     const grid_wave& _wave,
                     std::shared_ptr<potential<size_t>> _V,
                     std::shared_ptr<evo::integrator<grid_wave, H_obj> _evolver,
                     ) : qgridsystem<1D_H_matrix>(_m, _wave, _V, _evolver),
                        boundaries(_bounds),
                        // TODO, adjust correct dx
                        dx((_bounds.second - _bounds.first) / _wave.size()),
                        // compose the hemiltonian matrix
                        H(
                            composition((- hbar * hbar / (2 * mass() * dx * dx)) * A, 
                                        math::diag_functor([&] (size_t k) -> double { return V(k); } ))
                         )
        {
        }


void qsystem1D::set_mass(double m) {
    double old = mass();
    qgridsystem<1D_H_matrix>::set_mass(m);
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
