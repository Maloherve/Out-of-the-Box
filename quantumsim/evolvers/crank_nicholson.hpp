#pragma once

#include "evolver.hpp"
#include "grid/wave.hpp"

namespace qsim::evo {

    class crank_nicholson : public qsim::evolver {

    public:

        virtual grid::wave_vector evolve(const grid::qsystem1D& system, double dt) const override;
        virtual grid::wave_vector evolve(const grid::qsystem2D& system, double dt) const override;
    };
}
