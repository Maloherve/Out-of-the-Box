#include "grid/qsystem1D.hpp"
#include "constants.hpp"
#include "potential.hpp"

#include "grid/integrators.hpp"

using namespace qsim::grid;

const qsim::math::diagonals<double, 3> qsystem1D::A = math::diagonals<double, 3>({math::sdiag_entry(-1, 1.0), math::sdiag_entry(0, -2.0), math::sdiag_entry(1, 1.0)});

qsystem1D::qsystem1D(double _m, 
                     const std::pair<bound, bound>& _bounds,
                     const wave_vector& _wave,
                     std::shared_ptr<potential<size_t>> _V,
                     std::shared_ptr<evolver<size_t, wave_vector, grid_H_1D>> _evolver
                     ) : qgridsystem<H_matrix_1D>(_m, _wave, _V, _evolver),
                        boundaries(_bounds),
                        // compose the hemiltonian matrix
                        H(
                            0.0,
                            H_zero(), 
                            std::function<double (size_t)>([&] (size_t k) -> double { return V()(k); }) 
                         )
        {
            // check if boundaries are correct
            if (boundaries.first.location > boundaries.second.location)
                std::swap(boundaries.first, boundaries.second);
            
            // setup boundaries
            boundaries_setup();
        }


void qsystem1D::boundaries_setup() {
    // extend entering wave
    wave.insert(wave.begin(), 0.0);
    wave.push_back(0.0);

    post(0); // setup boundaries correctly
}

qsim::math::diagonals<double, 3> qsystem1D::H_zero() const {
    return math::diagonals<double,3>((- hbar * hbar / (2 * mass() * pow(dx(), 2))) * A);
}

void qsystem1D::update_H() {
    /*
     * Regenerate the part of the matrix which corresponds to 
     * the H_0 hamiltonian
     */
    H.get<1>() = H_zero();
}

void qsystem1D::set_mass(double _m) {
    qgridsystem<H_matrix_1D>::set_mass(_m);
    update_H();
}

double qsystem1D::normalize() {

    wave_t norm = grid::grid_integrate(wave, [&] (const neighbourhood<>& map, size_t location) {
                    return map.at(location, 0); // return the value itself
                }, dx());

    if (abs(norm.imag()) > qsim::machine_prec)
        throw norm; // TODO, a real error

    double out = sqrt(norm.real());
    
    wave /= out;  
    return out;
}

void qsystem1D::post(double) {
    
    switch (boundaries.first.mode) {
        case boundary_mode::free:
            wave.front() = 2.0 * wave[1u] - wave[2u];
            break;
        case boundary_mode::fixed:
            wave.front() = 0;
            break;
    }
    
    // last index
    size_t N = wave.size() - 2;

    switch (boundaries.second.mode) {
        case boundary_mode::free:
            wave.back() = 2.0 * wave[N] - wave[N-1];
            break;
        case boundary_mode::fixed:
            wave.back() = 0;
            break;
    }
}

std::vector<double> qsystem1D::generate_map() const {

    std::vector<double> out;
    out.reserve(size());

    for (size_t k = 0; k < size(); ++k)
        out.push_back(map(k));

    return out;
}

void qsystem1D::replace_wave(const wave_vector& other) {
    qgridsystem<H_matrix_1D>::replace_wave(other);
    boundaries_setup();
    update_H(); // update matrix
}

void qsystem1D::replace_wave(wave_vector&& other) {
    qgridsystem<H_matrix_1D>::replace_wave(other);
    boundaries_setup();
    update_H(); // update matrix
}

const std::pair<qsystem1D::bound, qsystem1D::bound>& qsystem1D::bounds() const {
    return boundaries;
}

double qsystem1D::map(size_t k) const {
    return boundaries.first + dx() * k;
}

void qsystem1D::set_upper_bound(double up) {
    if (up > boundaries.first) {
        boundaries.second = up;
        update_H();
    }
}

void qsystem1D::set_lower_bound(double low) {
    if (low < boundaries.second) {
        boundaries.first = low;
        update_H();
    }
}

// TODO, implement trapezium integration
double qsystem1D::energy() const {
    const double H0 = -hbar * hbar / (2*mass());
    wave_t result = grid::grid_integrate(wave, [&] (const neighbourhood<>& map, size_t location) {
                    return H0 * (map.at(location, -1) + map.at(location, 1))  
                        + (V()(location) - 2 * H0) * map.at(location, 0); // return the value itself
                }, dx());

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
