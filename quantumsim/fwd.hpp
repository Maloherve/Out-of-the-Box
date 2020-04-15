#pragma once

#include <cstddef>

namespace qsim {

    template <class Coords, class WaveFunction>
    class qsystem;

    class evolver;

    namespace grid {

        class wave_vector;
        class qsystem1D;
        class qsystem2D;

        typedef qsystem<size_t, wave_vector> qgridsystem;
    }
}
