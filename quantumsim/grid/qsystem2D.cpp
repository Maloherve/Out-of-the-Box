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

diag_functor<wave_t> qsystem2D::potential_on_row(size_t i) const {
    return std::function<wave_t (size_t)>([&] (size_t k) -> double { return V()(i,k); });
}

diag_functor<wave_t> qsystem2D::potential_on_column(size_t j) const {
    return std::function<wave_t (size_t)>([&] (size_t k) -> double { return V()(k,j); });
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

std::pair<double,double> qsystem2D::position() const {
    std::pair<double,double> out(0, 0);
    for (auto it = begin(); it != end(); ++it) {
        out.first += it.x() * std::norm(*it);
        out.second += it.y() * std::norm(*it);
    }

    out.first *= dx * dy;
    out.second *= dx * dy;
    return out;
}

std::pair<double,double> qsystem2D::momentum() const {
    qsim::wave_t px(0), py(0);
    for (auto it = begin(); it != end(); ++it) {
        px += it.right() - it.left();
        py += it.up() - it.down();
    }

    using namespace std::complex_literals;

    px *= - 1i * hbar() * dy / 2.0;
    py *= - 1i * hbar() * dx / 2.0;

    if (abs(px.imag()) > qsim::machine_prec || abs(py.imag()) > qsim::machine_prec) {
        npdebug("Values: (", px, ", ", py, ")")
        throw std::runtime_error("Momentum computation isn't fully real");
    }

    return std::pair<double, double>(px.real(), py.imag());
}

// normalize the wave function
double qsystem2D::norm() const {
    double out(0);
    for (const qsim::wave_t& val : *this)
        out += std::norm(val);

    return out * dx * dy;
}

size_t qsystem2D::N() const {
    return _N;
}
size_t qsystem2D::M() const {
    return _M;
}


qsystem2D::iterator qsystem2D::begin() {
    return iterator(*this, 1, 1);
}

qsystem2D::const_iterator qsystem2D::begin() const {
    return const_iterator(*this, 1, 1);
}

qsystem2D::iterator qsystem2D::end() {
    return iterator(*this, _N+1, 1);
}

qsystem2D::const_iterator qsystem2D::end() const {
    return const_iterator(*this, _N+1, 1);
}*/

/*
 * iterator section
 */ 
/*
qsystem2D::iterator::iterator(qsystem2D& _sys, size_t _i, size_t _j) 
    : sys(_sys), i(_i), j(_j)
{
}

void qsystem2D::iterator::increment() {
    if (j < sys.M())
        ++j;
    else {
        ++i;
        j = 1;
    }
}


bool qsystem2D::iterator::operator!=(const iterator& other) const {
    return i != other.i || j != other.j;
}

qsim::wave_t& qsystem2D::iterator::operator*() {
    return sys.wave[sys.map(i,j)];
}


qsim::wave_t& qsystem2D::iterator::up() {
    return sys.wave[sys.map(i,j+1)];
}

qsim::wave_t& qsystem2D::iterator::down() {
    return sys.wave[sys.map(i,j-1)];
}

qsim::wave_t& qsystem2D::iterator::left() {
    return sys.wave[sys.map(i-1,j)];
}

qsim::wave_t& qsystem2D::iterator::right() {
    return sys.wave[sys.map(i+1,j)];
}

double qsystem2D::iterator::x() const {
    return sys.x(i);
}

double qsystem2D::iterator::y() const {
    return sys.y(j);
}

// const iterator

qsystem2D::const_iterator::const_iterator(const qsystem2D& _sys, size_t _i, size_t _j) 
    : sys(_sys), i(_i), j(_j)
{
}

void qsystem2D::const_iterator::increment() {
    if (j < sys.M())
        ++j;
    else {
        ++i;
        j = 1;
    }
}

bool qsystem2D::const_iterator::operator!=(const const_iterator& other) const {
    return i != other.i || j != other.j;
}

const qsim::wave_t& qsystem2D::const_iterator::operator*() const {
    return sys.wave[sys.map(i,j)];
}


const qsim::wave_t& qsystem2D::const_iterator::up() const {
    return sys.wave[sys.map(i,j+1)];
}

const qsim::wave_t& qsystem2D::const_iterator::down() const {
    return sys.wave[sys.map(i,j-1)];
}

const qsim::wave_t& qsystem2D::const_iterator::left() const {
    return sys.wave[sys.map(i-1,j)];
}

const qsim::wave_t& qsystem2D::const_iterator::right() const {
    return sys.wave[sys.map(i+1,j)];
}

double qsystem2D::const_iterator::x() const {
    return sys.x(i);
}

double qsystem2D::const_iterator::y() const {
    return sys.y(j);
}*/

