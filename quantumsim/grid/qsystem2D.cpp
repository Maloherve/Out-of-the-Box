#include "grid/qsystem2D.hpp"
#include "constants.hpp"
#include "potential.hpp"

#include "grid/integrators.hpp"

#include "debug.hpp"

using namespace qsim::grid;
using namespace qsim::math;

const qsim::math::diagonals<wave_t, 3> qsystem2D::A = math::diagonals<wave_t, 3>({math::sdiag_entry(-1, 1.0), math::sdiag_entry(0, -2.0), math::sdiag_entry(1, 1.0)});

// init pack
wave_grid qsystem2D::init_pack::generate(double dx, double dy) const {
    matrix<wave_t> psi(N, M, wave_t(0.0));

    // construct it using the analytic expression
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j)
            psi.at(i,j) = f(i * dx, j * dy);
    }

    return psi;
}

// constructor
qsystem2D::qsystem2D(double _m, 
                     double _dx, double _dy,
                     std::shared_ptr<potential<size_t,size_t>> _V,
                     const init_pack& init,
                     std::shared_ptr<evolver> _evolver,
                     double hbar
                    )
    : qbi_gridsystem(_m, init.generate(_dx,_dy), _V, _evolver, hbar),
      dx(_dx), dy(_dy)
{
    normalize();
}

// allow to set boundaries
void qsystem2D::evolve(double dt) {
    wave = m_evolver->evolve(*this, dt);
}

math::diagonals<wave_t, 3> qsystem2D::H_zero_x() const {
    return (-pow(hbar() / dx, 2)) * A;
}

math::diagonals<wave_t, 3> qsystem2D::H_zero_y() const {
    return (-pow(hbar() / dy, 2)) * A;
}

math::diagonals<wave_t, 2> qsystem2D::Px() const {
    using namespace std::complex_literals;
    return (-1i * hbar() / (2.0 * dx)) * math::diagonals<wave_t, 2>({-1, -1}, {1, 1});
}

math::diagonals<wave_t, 2> qsystem2D::Py() const {
    using namespace std::complex_literals;
    return (-1i * hbar() / (2.0 * dy)) * math::diagonals<wave_t, 2>({-1, -1}, {1, 1});
}

diag_functor<wave_t> qsystem2D::potential_on_row(size_t i) const {
    return std::function<wave_t (size_t)>([&] (size_t k) -> double { return V()(i,k); });
}

diag_functor<wave_t> qsystem2D::potential_on_column(size_t j) const {
    return std::function<wave_t (size_t)>([&] (size_t k) -> double { return V()(k,j); });
}


// normalize the wave function
double qsystem2D::norm() const {
    double out(0);
    for (size_t i = 0; i < N(); ++i) {
        for (size_t j = 0; j < M(); ++j) {
            out += std::norm(wave(i,j));
        }
    }

    return out * dx * dy;
}

std::pair<double,double> qsystem2D::position() const {
    std::pair<double,double> out(0, 0);
    for (size_t i = 0; i < N(); ++i) {
        for (size_t j = 0; j < M(); ++j) {
            double val = std::norm(wave(i,j));
            out.first += x(i) * val;
            out.second += y(j) * val;
        }
    }

    out.first *= dx * dy;
    out.second *= dx * dy;
    return out;
}

std::pair<double,double> qsystem2D::momentum() const {
    qsim::wave_t px(0), py(0);
    for (size_t i = 0; i < N(); ++i)
        px += std::conj(wave.get_row(i)) * (P() * wave.get_row(i));

    for (size_t j = 0; j < M(); ++j)
        py += std::conj(wave.get_column(i)) * (P() * wave.get_column(i));

    using namespace std::complex_literals;

    //px *= - 1i * hbar() * dy / 2.0;
    //py *= - 1i * hbar() * dx / 2.0;
    px *= dx * dy;
    py *= dx * dy;

    if (abs(px.imag()) > qsim::machine_prec || abs(py.imag()) > qsim::machine_prec) {
        npdebug("Values: (", px, ", ", py, ")")
        throw std::runtime_error("Momentum computation isn't fully real");
    }

    return std::pair<double, double>(px.real(), py.real());
}


/*
 * TODO, all below this
 */
// implementations
/*double qsystem2D::energy() const {
    qsim::wave_t E(0);
    double psix = pow(hbar()/dx,2) / (2 * mass());
    double psiy = pow(hbar()/dy,2) / (2 * mass());
    double v = 2 * (psix + psiy);
    
    // apply the hamiltonian
    for (auto it = begin(); it != end(); ++it) {
        E += (this->V()(it.k()) + v) * (*it) 
           - psix * (it.right() + it.left()) 
           - psiy * (it.up() + it.down());
    }

    if (abs(E.imag()) > qsim::machine_prec) {
        npdebug("Value: ", E)
        throw std::runtime_error("Energy computation isn't fully real");
    }

    return E.real();
}


