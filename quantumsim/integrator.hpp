#pragma once

namespace qsim {

    template <typename Coords, template <typename _Coords> class WaveFunction<_Coords>>
    class qsystem;
}

namespace qsim::evo {
    
    template <typename Coords, template <typename _Coords> class WaveFunction<_Coords>, class H>
    class integrator {
    public:

        virtual WaveFunction<Coords> evolve(const qsystem<Coords, WaveFunction<Coords>, H>& system, double dt) const = 0;
    };
}
