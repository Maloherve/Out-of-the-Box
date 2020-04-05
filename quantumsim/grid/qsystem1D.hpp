#pragma once

#include "grid/qgridsystem.hpp"
#include "math/composition.hpp"
#include "math/diagonals.hpp"
#include "math/diagonal.hpp"
#include "evolvers/evofw.hpp"

namespace qsim::grid {

    typedef qsim::math::composition<
                              double, 
                              math::diagonals<double, 3>, 
                              math::diag_functor<double>
                             > H_matrix_1D;

    typedef grid_H<H_matrix_1D> grid_H_1D;
    

    // concretization for a 1D grid
    class qsystem1D : public qgridsystem<H_matrix_1D> {
        
        // hamiltonian object
        const std::pair<double, double> boundaries;
        const double dx;

        H_matrix_1D H; // non-constant, the mass could change
        
        // standard matrix A
        static const math::diagonals<double, 3> A;

    public: 

        // possible integrators' forward declaration
        typedef evo::explicit_scheme<size_t, wave_vector, grid_H_1D> explicit_evolver;

        qsystem1D(double _m, 
                  const std::pair<double, double>& _bounds,
                  wave_vector* _wave,
                  std::shared_ptr<potential<size_t>> _V,
                  std::shared_ptr<evolver<size_t, wave_vector, grid_H_1D>> _evolver
                  );

        virtual const H_matrix_1D* hemiltonian_ptr() const {
            return &H;
        }        

        
        // change the hemiltonian expression
        virtual void set_mass(double) override;
        
        // implementations
        virtual double energy() const override;
        virtual double position() const override;
        virtual double momentum() const override;

        // normalize the wave function
        virtual double normalize() override;

        std::vector<double> generate_map() const;

        double map(size_t) const;
    };
}

