#include "crank_nicholson.hpp"

#include "grid/qsystem1D.hpp"
#include "grid/qsystem2D.hpp"

#include "debug.hpp"

using namespace qsim::grid;
using namespace qsim::evo;

using namespace std::complex_literals;

wave_vector solve1D(const H_matrix_1D& A, wave_vector);

wave_vector crank_nicholson::evolve(const qsystem1D& system, double dt) const {
    auto H = system.H();
    // deduce y vector
    wave_vector y = (wave_t(1.0) - (1i * dt / (2 *system.hbar())) * H) * system.psi();
    // solve y = ( 1 + i * dt / (2 * hbar) * H ) * x 
    wave_vector x = solve1D((wave_t(1.0) + (1i * dt / (2 *system.hbar())) * H), y);
    return x;
}

wave_vector crank_nicholson::evolve(const qsystem2D& system, double dt) const {
    return system.psi();
}

/*
 * Gauss jordan solver with the following hypothesis:
 *    - A is tridiagonal
 */
wave_vector solve1D(const H_matrix_1D& A, wave_vector x) {
	wave_vector new_diag(x.size(), 0.0);

    new_diag[0] = A.at(0,0);
    
    for(std::size_t i(1); i < x.size(); ++i) {

	    wave_t pivot = A.at(i-1, i) / A.at(i-1, i-1);
	    x[i] -= x[i-1] * pivot;
	    new_diag[i] = A.at(i,i) - A.at(i, i-1) * pivot;
	}

	for(std::size_t i(x.size()-1); i > 0; --i) {
	    x[i] /= new_diag[i];
        x[i-1] -= A.at(i-1, i) * x[i];
	}
    
    x[0u] /= new_diag[0u];

	return x;
}


