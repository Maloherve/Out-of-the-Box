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
    public:
        struct bound {
            double location;
            boundary_mode mode;

            bound(double x, boundary_mode md = boundary_mode::free) 
                : location(x), mode(md) {}

            operator double() const {
                return location;
            }
        };

    private:
        // hamiltonian object
        std::pair<bound, bound> boundaries;

        H_matrix_1D H; // non-constant, the mass could change
        
        // standard matrix A
        static const math::diagonals<double, 3> A;
        
        // determine first hamiltonian term in function of the mass and the discretization step
        qsim::math::diagonals<double, 3> H_zero() const;

        void boundaries_setup();

    public: 

        // possible integrators' forward declaration
        typedef evo::explicit_scheme<size_t, wave_vector, grid_H_1D> explicit_evolver;

        qsystem1D(double _m, 
                  const std::pair<bound, bound>& _bounds,
                  const wave_vector& _wave,
                  std::shared_ptr<potential<size_t>> _V,
                  std::shared_ptr<evolver<size_t, wave_vector, grid_H_1D>> _evolver
                  );

        virtual const H_matrix_1D* hemiltonian_ptr() const {
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
            return qgridsystem<H_matrix_1D>::size() - 2;
        }

        /*
         * Access to boundaries
         */

        const std::pair<bound, bound>& bounds() const;

        inline bound& lower_bound() {
            return boundaries.first;
        }

        inline bound& upper_bound() {
            return boundaries.second;
        }

        inline bound lower_bound() const {
            return boundaries.first;
        }

        inline bound upper_bound() const {
            return boundaries.second;
        }

        void set_upper_bound(double up);
        void set_lower_bound(double low);

        inline double dx() const {
            return (boundaries.second - boundaries.first) / size();
        }

        std::vector<double> generate_map() const;

        double map(size_t) const;
    };
}

