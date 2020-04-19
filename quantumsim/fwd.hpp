#pragma once

#include <cstddef>

namespace qsim {

    template <class WaveFunction, typename ...Coords>
    class qsystem;

    class evolver;

    namespace math {
        template <typename T>
        class matrix;
    }

    namespace grid {

        class wave_vector;
        class qsystem1D;
        class qsystem2D;

        typedef qsystem<wave_vector, size_t> qgridsystem;
        typedef qsystem<math::matrix<wave_t>, size_t, size_t> qbi_gridsystem;
    }
}
