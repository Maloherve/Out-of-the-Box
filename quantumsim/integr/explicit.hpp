#pragma once

#include "integrator.hpp"

namespace qsim::evo {

    template <typename Coords, template <typename _Coords> class WaveFunction<_Coords>, class H>
    class explicit_scheme : public integrator<Coords, WaveFunction<Coords>, H> {
    public:

        virtual WaveFunction<Coords> evolve(const qsystem<Coords, WaveFunction<Coords>, H>& system, double dt) const {
            return (1 - (dt / hbar) * system.hemiltonian()) * system.psi();
        }
    };
}
