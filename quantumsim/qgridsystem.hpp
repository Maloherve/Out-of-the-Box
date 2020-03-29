#pragma once 

#include "qsystem.hpp"
#include <array>
#include "constants.hpp"

namespace qsim {
   
    // type definitions 
    typedef size_t grid_t;

    template <grid_t Ni, grid_t Nj>
    using wave_t =  std::array<std::complex, Ni * Nj>;

    // evolvers
    namespace evo {
        
        // explicit time differentiation functor
        template <grid_t Ni, grid_t Nj>
        struct grid_explicit {

            static constexpr grid_t N = Ni * Nj;

            template<class H>
            wave_t<Ni, Nj> operator()(const wave_t<Ni, Nj>& psi, H&& h, double dt) {

                wave_t<Ni, Nj> out;
                constexpr long int cset[] = {Nj, -Nj, 1, -1, 0};

                for (grid_t m = 0; m < psi.size(); ++m) {
                    // select the hamiltonian where it's not zero
                    out[m] = 0;
                    for (long int c : cset) {
                        // careful to array boundaries
                        if (c + static_cast<long int>(m) >= 0 && 
                            c + static_cast<long int>(m) < psi.size())
                            out[m] += h(m, c+m) * psi[c+m];
                    }
                }

                return out;
            }
        };
    }

    /*
     * System approximation using a finite difference spatial approximation
     */
    template <grid_t Ni, grd_t Nj, class Integrator<Ni,Nj> = evo::grid_explicit<Ni,Nj>>
    class qgridsystem : qsystem<grid_t> {

        // Wavefunction discretization
        wave_t<Ni, Nj> grid;

        // an abstract integrator, a friend functor
        Integrator<Ni,Nj> evolver;
        
        // discretization intervals
        const double dx, dy;

    public: 
        qgridsystem(double Lx, double Ly, double m, 
                    wave_t<Ni, Nj> init,
                    std::shared_ptr<potential<grid_t>> _V, 
                    Integrator<Ni,Nj> _evolver = evo::grid_explicit<Ni,Nj>())
            : qsystem(m, _V), evolver(_evolver), dx(Lx/Ni), dy(Ly/Nj) {}

        static constexpr grid_t Nx = Ni;
        static constexpr grid_t Ny = Nj;
        
        double length() const {
            return dx * Nx;
        }

        double height() const {
            return dy * Ny;
        }
        
        // convertion from (i,j) to m
        static constexpr grid_t m(size_t i, size_t j) {
            return i * Nj + j;
        }

        virtual std::complex psi(grid_t m) const override {
            return grid[m];
        }

        virtual void evolve(double dt) override {
            // hemiltonian matrix descriptor
            auto&& H = [&] -> double (grid_t m, grid_t c) {
                if (c == m)
                    return V(m) + hbar * hbar / m * (1 / (dx*dx) + 1 / (dy*dy));
                else if (c == m + Nj || c == m - Nj)
                    return - hbar * hbar / (2 * mass() * dx * dx);
                else if (c == m + 1 || c == m - 1)
                    return - hbar * hbar / (2 * mass() * dy * dy);

                return 0; // else
            };
            
            // replace the grid with the new one
            grid = evolver(grid, std::move(H), dt);
        }
    };
}
