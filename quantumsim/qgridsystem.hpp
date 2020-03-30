#pragma once 

#include "qsystem.hpp"
#include <array>
#include "constants.hpp"
#include "diagonals.hpp"

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

                for (grid_t m = 0; m < psi.size(); ++m) {
                    out[m] = 0;
                    for (const auto& row : h(m))
                        out[m] += row.value() * psi[row.column()];
                }

                return out;
            }
        };
    }

    /*
     * System approximation using a finite difference spatial approximation
     */
    template <grid_t Ni, grid_t Nj, class Integrator<Ni,Nj> = evo::grid_explicit<Ni,Nj>>
    class qgridsystem : qsystem<grid_t> {

        // Wavefunction discretization
        wave_t<Ni, Nj> grid;

        // an abstract integrator, a friend functor
        Integrator<Ni,Nj> evolver;
        
        // discretization intervals
        const double dx, dy;

        // laplace operator
        const math::diagonal<double, 5, Ni*Nj> laplace;

    public: 
        qgridsystem(double Lx, double Ly, double m, 
                    wave_t<Ni, Nj> init,
                    std::shared_ptr<potential<grid_t>> _V, 
                    Integrator<Ni,Nj> _evolver = evo::grid_explicit<Ni,Nj>())
            : qsystem(m, _V), evolver(_evolver), dx(Lx/Ni), dy(Ly/Nj),
              laplace({std::pair<long int, double>(-Nj, 1/(dx*dx)), 
                       std::pair<long int, double>(-1, 1/(dy*dy)),
                       std::pair<long int, double>(1, 1/(dy*dy)),
                       std::pair<long int, double>(0, 2/(dx*dx) + 2/(dy*dy)),
                       std::pair<long int, double>(Nj, 1/(dx*dx))}) { 
            }

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
       
        /* 
         * returns a diagonals::row_array, complexity O(1)
         * this is a descriptor of the hemiltonian matrix storing only the minimal quantity if values
         * A diagonals::row_array is iterable object of which value in this case is a std::pair<grid_t, double>, where:
         *  ->first : column index
         *  ->second : value of the matrix at the specified row and the column of "->first" 
         */
        inline auto H(grid_t m) const {
           return (((- hbar * hbar / (2 * mass())) * laplace) += V(m)).get_row();
        }

        virtual void evolve(double dt) override {
            
            // replace the old wave function grid with the new one
            grid = evolver(grid,
                           [&](grid_t m) -> auto { return H(m) }, 
                           dt);
        }
    };
}
