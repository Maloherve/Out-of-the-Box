#pragma once

namespace qsim {

    template <typename Coords, class WaveFunction, class H>
    class qsystem;
}

namespace qsim {
    
    template <typename Coords, class WaveFunction, class H>
    class evolver {
    public:

        virtual WaveFunction evolve(const qsystem<Coords, WaveFunction, H>& system, double dt) const = 0;
    };
}
