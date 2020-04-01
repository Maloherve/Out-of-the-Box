#pragma once

#include "qsystem.hpp"
#include "math/ptr_composition.hpp"
#include "wave.hpp"

namespace qsim {

// template helper macros, qgs = qgridsystem
#define qgs_coords size_t
#define qgs_wave qsim::grid_wave
#define qgs_H(H) qsim::math::ptr_composition<double, H>

    template<class H>
    class qgridsystem : public qsystem<size_t, grid_wave, math::ptr_composition<double, H>> {

    protected:

        using H_obj = math::ptr_composition<double, H>;
        
    public:

        using qsystem<size_t, grid_wave, H_obj>::qsystem;
        
        // do not copy real hemiltonian
        virtual H_obj hemiltonian() const override {
            return H_obj({hemiltonian_ptr()});
        }
        
        // this value depends on the exact grid
        virtual const H* hemiltonian_ptr() const = 0;

        inline size_t size() const {
            return this->psi().size();
        }
    };

}
