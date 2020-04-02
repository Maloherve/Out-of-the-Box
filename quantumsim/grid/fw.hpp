#pragma once

#include "math/ptr_composition.hpp"
#include "math/composition.hpp"

namespace qsim::grid {

// template helper macros, qgs = qgridsystem
#define qgs_coords size_t
#define qgs_H(H) qsim::math::ptr_composition<double, H>

    // grid coordinates
    typedef grid_t size_t;
    
    // hemiltonian class 
    template<class H>
    using grid_H = qsim::math::ptr_composition<double, H>;

        // macro helper for templates deduction
    #define qgs1_H qsim::H_matrix_1D
    #define qgs1_args(type) type<size_t, qsim::grid_wave, qsim::math::ptr_composition<double, qgs1_H>>
}

