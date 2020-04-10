#pragma once

#include "grid/qgridsystem.hpp"
#include "math/composition.hpp"
#include "math/diagonals.hpp"
#include "math/diagonal.hpp"
#include "evolvers/evofw.hpp"

namespace qsim::grid {

    typedef qsim::math::composition<
                              double, 
                              math::diagonals<double, 3>, //dx
                              math::diagonals<double, 3>, //dy
                              math::diag_functor<double>
                             > H_matrix_2D;

    typedef grid_H<H_matrix_2D> grid_H_2D;
    

    // concretization for a 2D grid
    class qsystem2D : public qgridsystem<H_matrix_2D> {
    private:

        // hamiltonian object
        H_matrix_2D H; // non-constant, the mass could change
        
        // standard matrix A
        static const math::diagonals<double, 3> A_x;
        static const math::diagonals<double, 3> A_y;

        double dx; // discretization along x
        double dy; // discretization along y
        
        // determine first hamiltonian term in function of the mass and the discretization step
        qsim::math::diagonals<double, 3> H_zero_x() const;
        qsim::math::diagonals<double, 3> H_zero_y() const;

    public: 

        // possible integrators' forward declaration
        typedef evo::explicit_scheme<size_t, wave_vector, 2D> explicit_evolver;

        qsystem2D(double _m, 
                  const wave_vector& _wave,
                  std::shared_ptr<potential<size_t>> _V,
                  std::shared_ptr<evolver<size_t, wave_vector, grid_H_2D>> _evolver
                  );

        virtual const H_matrix_2D* hemiltonian_ptr() const {
            return &H;
        }        
        
        // update the hamiltonian matrix
        void update_H();
        
        // change the hemiltonian expression
        virtual void set_mass(double) override;

        // allow to set boundaries
        virtual void post(double) override;
        
        // implementations
        virtual double energy() const override;
        virtual double position() const override;
        virtual double momentum() const override;

        // normalize the wave function
        virtual double normalize() override;
        
        // override these functions
        void replace_wave(const wave_vector& other);
        void replace_wave(wave_vector&& other);

        /*
         * Causing of boundaries size is decreased of 2
         */

        inline size_t size() const {
            return qgridsystem<H_matrix_2D>::size();
        }
    };
}
