#pragma once

#include "evolver.hpp"
#include "constants.hpp"

namespace qsim::evo {

    template <typename Coords, class WaveFunction, class H>
    class explicit_scheme : public qsim::evolver<Coords, WaveFunction, H> {

    public:

        virtual WaveFunction evolve(const qsystem<Coords, WaveFunction, H>& system, double dt) const {
            return (1.0 - (dt / hbar) * system.hemiltonian()) * system.psi();
        }
    };
}
