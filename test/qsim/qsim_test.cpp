#include <iostream>

#include "grid/qsystem1D.hpp"
#include "grid/wave.hpp"
#include "potentials/uniform.hpp"
#include "evolvers/explicit.hpp"
#include "grid/integrators.hpp"

#include <fstream>
#include <sstream>

using namespace qsim;
using namespace qsim::grid;
using namespace std;

wave_vector init_wave();
void print(const qsystem1D&, const std::vector<double>&, size_t k);

constexpr double mass = 1;
constexpr double dt = 1;
constexpr std::pair<double, double> bounds = {-50, 50};
constexpr size_t steps = 100;

int main() {
    
    // flat zero potential
    auto V = std::make_shared<pot::uniform<size_t>>();
    auto integ = std::make_shared<qsystem1D::explicit_evolver>();
    
    // step 1, initialization
    qsystem1D system(mass, bounds, init_wave(), V, integ);
    
    try {
        system.normalize();
    } catch (wave_t A) {
        cout << "Normalization: " << A << endl;
    }

    /*
     * Print out the discretization 1D map
     */
    auto map = system.generate_map(); 
    
    /*
     * Symulate
     */
    for (size_t k = 0; k < steps; k++) {
        // print out psi
        print(system, map, k);
        // evolve
        system.evolve(dt); 

        double A = system.normalize();
        cout << "Step k, norm = " << A << endl;
    }
}

wave_vector init_wave() {

    using namespace std::complex_literals;

    constexpr size_t N = 50;

    wave_vector v(N);
    constexpr double L = bounds.second - bounds.first;
    constexpr double s_norm = 0.4;
    constexpr double x0 = 0;
    constexpr double n = 1;
    constexpr double sigma(s_norm * L);
    constexpr double k0(2 * M_PI * n / L);

    for (size_t k = 0; k < N; ++k) {
        double x = bounds.first + k * L / N;
        v[k] = exp(1i * k0 * x - pow((x - x0)/sigma, 2) / 2);
    }

    return v;
}

void print(const qsystem1D& system, const std::vector<double>& map, size_t k) {

    std::ostringstream filename;
    filename << "outs/wave_test" << k << ".out";
    ofstream out(filename.str());
 
    for (size_t i = 0; i < map.size(); ++i)
        out << map[i] << "  " << norm(system.psi()[i]) << endl;

    out.close();
}

