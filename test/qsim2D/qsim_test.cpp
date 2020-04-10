#include <iostream>

#include "grid/qsystem1D.hpp"
#include "grid/wave.hpp"
#include "potentials/uniform.hpp"
#include "potentials/stepped.hpp"
#include "evolvers/explicit.hpp"
#include "grid/integrators.hpp"

#include <fstream>
#include <sstream>

using namespace qsim;
using namespace qsim::grid;
using namespace std;

wave_vector init_wave(size_t);
void print(const qsystem1D&, const std::vector<double>&, size_t k);

constexpr double mass = 1;
constexpr double dt = 0.01;
constexpr std::pair<double, double> bounds = {-50, 50};
constexpr size_t steps = 1000;

constexpr size_t samples = 100;

int main() {
    
    // flat zero potential
    auto V_flat = std::make_shared<pot::uniform<size_t>>();

    constexpr double position = 0.75;

    // barrier in the middle
    auto V_barrier = std::make_shared<pot::stepped<size_t>>(5, samples * (position - 1.0/20), samples * (position + 1.0/20));

    auto integ = std::make_shared<qsystem1D::explicit_evolver>();
    
    // step 1, initialization
    qsystem1D system(mass, bounds, init_wave(samples), V_barrier, integ);
    
    double A;
    try {
        A = system.normalize();
    } catch (wave_t val) {
        cout << "Normalization: " << val << endl;
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
        if (k % 20 == 0) {
            print(system, map, k);
            double E = system.energy();
            cout << "Step k, norm = " << A << " , E = " << E << endl;
        }

        // evolve
        system.evolve(dt); 
        A = system.normalize();
    }
}

wave_vector init_wave(size_t size) {

    using namespace std::complex_literals;

    wave_vector v(size);
    constexpr double L = bounds.second - bounds.first; // half side
    constexpr double s_norm = 0.05; // peak length
    constexpr double x0 = -25;
    constexpr double n = 16;
    constexpr double sigma(s_norm * L / 2);
    constexpr double k0(2 * M_PI * n / L);

    for (size_t k = 0; k < size; ++k) {
        double x = bounds.first + double(k * L) / size;
        v[k] = exp(1i * k0 * x - pow((x - x0)/sigma, 2) / 2);
    }

    return v;
}

void print(const qsystem1D& system, const std::vector<double>& map, size_t k) {

    std::ostringstream filename;
    filename << "outs/wave_test" << k << ".out";
    ofstream out(filename.str());
 
    for (size_t i = 0; i < map.size(); ++i)
        out << map[i] << "  " << real(system.psi()[i]) << "  " << imag(system.psi()[i]) << "  " << norm(system.psi()[i]) << endl;

    out.close();
}

