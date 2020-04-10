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
    private:

        // hamiltonian object
        H_matrix_1D H; // non-constant, the mass could change
        
        // discretization
        double dx;
        
        // standard matrix A
        static const math::diagonals<double, 3> A;
        
        // determine first hamiltonian term in function of the mass and the discretization step
        qsim::math::diagonals<double, 3> H_zero() const;

    public: 

        // possible integrators' forward declaration
        typedef evo::explicit_scheme<size_t, wave_vector, grid_H_1D> explicit_evolver;

        qsystem1D(double _m, 
                  const wave_vector& _wave,
                  double dx,
                  std::shared_ptr<potential<size_t>> _V,
                  std::shared_ptr<evolver<size_t, wave_vector, grid_H_1D>> _evolver,
                  double hbar = 1
                  );

        virtual const H_matrix_1D* hemiltonian_ptr() const {
            return &H;
        }        
        
        // update the hamiltonian matrix
        void update_H();
        
        // change the hemiltonian expression
        virtual void set_mass(double) override;

        // implementations
        virtual double energy() const override;
        double position() const;
        double momentum() const;

        // normalize the wave function
        virtual double norm() const override;
        
        // override these functions
        void replace_wave(const wave_vector& other);
        void replace_wave(wave_vector&& other);

        inline size_t size() const {
            return qgridsystem<H_matrix_1D>::size();
        }

        /*
         * Access to boundaries
         * Idea: i = -1 -> x = 0
         */

        inline double x(size_t i) const {
            return dx * (i+1);
        }
    };
}

