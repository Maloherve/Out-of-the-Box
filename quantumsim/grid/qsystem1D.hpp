#pragma once

#include "math/composition.hpp"

namespace qsim {

    typedef math::composition<double, math::diagonals<double, 3>, math::diag_functor<double>> 1D_H_matrix;
    
    // concretization for a 1D grid
    class qsystem1D : public qgridsystem<1D_H_matrix> {
        
        // hamiltonian object
        const std::pair<double, double> boundaries;
        const double dx;

        1D_H_matrix H; // non-constant, the mass could change
        
        // standard matrix A
        static constexpr math::diagonals<double, 3> A = 
            math::diagonals<double, 3>(math::sdiag_entry(-1, -1), math::sdiag_entry(0, 2), math::sdiag_entry(1, -1));

    public: 

        qsystem1D(double _m, 
                  const std::pair<double, double>& _bounds,
                  const grid_wave& _wave,
                  std::shared_ptr<potential<size_t>> _V,
                  std::shared_ptr<evo::integrator<grid_wave, H_obj> _evolver,
                  );

        virtual const 1D_H_matrix* hemiltonian_ptr() const {
            return &H;
        }        
        
        // change the hemiltonian expression
        virtual void set_mass(double m) override;
        
        // implementations
        virtual double energy() const override;
        virtual double position() const override;
        virtual double momentum() const override;
    };
}

