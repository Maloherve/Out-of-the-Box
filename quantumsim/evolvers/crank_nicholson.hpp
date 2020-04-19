#pragma once

#include "evolver.hpp"
#include "grid/wave.hpp"

namespace qsim::evo {

    class crank_nicholson : public qsim::evolver {

    public:
        
        /*
         * Solve the half-implicit 1D scheme
         */
        virtual grid::wave_vector evolve(const grid::qsystem1D& system, double dt) const override;

        /*
         * Approximate to a separable evolving equation
         */
        virtual grid::wave_vector evolve(const grid::qsystem2D& system, double dt) const override;
    };
}
