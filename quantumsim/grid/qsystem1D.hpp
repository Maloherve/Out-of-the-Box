#pragma once

#include "grid/qgridsystem.hpp"
#include "math/composition.hpp"
#include "math/diagonals.hpp"
#include "math/diagonal.hpp"

namespace qsim {

    typedef math::composition<
                              double, 
                              math::diagonals<double, 3>, 
                              math::diag_functor<double>
                             > H_matrix_1D;
    
// macro helper for templates deduction
#define qgs1_H qsim::H_matrix_1D
#define qgs1_args(type) type<size_t, qsim::grid_wave, qsim::math::ptr_composition<double, qgs1_H>>

    // concretization for a 1D grid
    class qsystem1D : public qgridsystem<H_matrix_1D> {
        
        // hamiltonian object
        const std::pair<double, double> boundaries;
        const double dx;

        H_matrix_1D H; // non-constant, the mass could change
        
        // standard matrix A
        static const math::diagonals<double, 3> A;

    public: 

        qsystem1D(double _m, 
                  const std::pair<double, double>& _bounds,
                  const grid_wave& _wave,
                  std::shared_ptr<potential<size_t>> _V,
                  std::shared_ptr<evo::integrator<size_t, grid_wave, H_obj>> _evolver
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
    };
}

