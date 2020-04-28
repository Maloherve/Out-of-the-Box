#include <iostream>

#include "grid/qsystem2D.hpp"
#include "math/matrix.hpp"
#include "potentials/uniform.hpp"
#include "evolvers/crank_nicholson.hpp"

#include <fstream>
#include <sstream>

using namespace qsim;
using namespace qsim::grid;
using namespace std;

qsystem2D::init_pack init_wave(size_t, size_t);
wave_t gauss(double x, double y);

constexpr double mass = 1;
constexpr double dt = 0.017;
constexpr double L = 100;
constexpr size_t steps = 1;

constexpr size_t N = 25;

constexpr double dx = L / N;
constexpr double dy = L / N;

int main() {
    
    // flat zero potential
    auto V_flat = std::make_shared<pot::uniform<size_t, size_t>>();

    constexpr double position = 0.75;

    auto integ = std::make_shared<qsim::evo::crank_nicholson>();
    
    // step 1, initialization
    qsystem2D system(mass, dx, dy, V_flat, init_wave(N,N), integ);
    npdebug("System constructed: norm = ", system.norm())

    /*
     * Symulate
     */
    for (size_t k = 0; k < steps; k++) {
        // print out psi
        if (k % 20 == 0) {
            double E = system.energy();
            double A = system.norm();
            cout << "Step k, norm = " << A << " , E = " << E << endl;
        }

        // evolve
        system.evolve(dt); 
        //system.normalize();
    }
}

constexpr double s_norm = 0.05; // peak length
constexpr double x_0 = L / 2;
constexpr double y_0 = L / 2;
constexpr double n = 16;
constexpr double m = 16;
constexpr double sigma(s_norm * L / 2);
constexpr double kx(2 * M_PI * n / L);
constexpr double ky(2 * M_PI * m / L);

wave_t gauss(double x, double y) {
    using namespace std::complex_literals;
    const double Dx = x - x_0;
    const double Dy = y - y_0;
    return exp(1i * (kx * Dx + ky * Dy) - (pow(Dx, 2) + pow(Dy, 2)) / (2.0 * pow(sigma,2)));
}

qsystem2D::init_pack init_wave(size_t _N, size_t _M) {

    return qsystem2D::init_pack(&gauss, _N, _M);
}

