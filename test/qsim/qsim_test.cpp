#include <iostream>

#include "grid/qsystem1D.hpp"
#include "grid/wave.hpp"
#include "potentials/uniform.hpp"
#include "evolvers/crank_nicholson.hpp"

#include <fstream>
#include <sstream>

using namespace qsim;
using namespace qsim::grid;
using namespace std;

qsystem1D::init_pack init_wave(size_t);

constexpr double mass = 1;
constexpr double dt = 0.017;
constexpr double L = 1;
constexpr size_t steps = 500;

constexpr size_t samples = 100;

constexpr double dx = L / (samples+1);

int main() {
    
    // flat zero potential
    auto V_flat = std::make_shared<pot::uniform<size_t>>();

    constexpr double position = 0.75;

    // barrier in the middle
    //auto V_barrier = std::make_shared<pot::stepped<size_t>>(5, samples * (position - 1.0/20), samples * (position + 1.0/20));

    auto integ = std::make_shared<qsim::evo::crank_nicholson>();
    
    // step 1, initialization
    qsystem1D system(mass, dx, V_flat, init_wave(samples), integ);

    /*
     * Symulate
     */
    for (size_t k = 0; k < steps; k++) {
        // print out psi
        if (k % 20 == 0) {
            double A = system.norm();
            double E = system.energy();
            cout << "Step k, norm = " << A << " , E = " << E << endl;
        }

        // evolve
        system.evolve(dt); 
        //system.normalize();
    }
}

constexpr double x0 = L / 2;
constexpr double n = 0;
constexpr double sigma(L / 10);
constexpr double k0(2 * M_PI * n / L);

wave_t gauss(double x) {
    using namespace std::complex_literals;
    return exp(1i * k0 * x - pow((x - x0)/sigma, 2) / 2);
}

qsystem1D::init_pack init_wave(size_t size) {

    return qsystem1D::init_pack(&gauss, size);
}

