#pragma once

#include <Godot.hpp>
#include <Object.hpp>
#include <Vector2.hpp>
#include "quantumsim/grid/fw.hpp"

namespace godot {

    class grid_wave : public Object {
        GODOT_CLASS(grid_wave, Object)

    protected: 
        qsim::grid::wave_vector * wave_ref;

        inline const qsim::grid::wave_vector * const_wave() const {
            return const_cast<const qsim::grid::wave_vector*>(wave_ref);
        }

    public:

        grid_wave(qsim::grid::wave_vector * wave = nullptr)
            : wave_ref(wave) {}
        ~grid_wave() = default;
        
        /*
         * initializers
         */
        void _init() {}
        static void _register_methods() {}
    };

    class grid_wave1D : public grid_wave {
        GODOT_SUBCLASS(grid_wave1D, grid_wave)
        friend class qgridsystem1D;

    public:
        using grid_wave::grid_wave;

        /*
         * Methods to be bounded
         */
        double real(int) const;
        double imag(int) const;
        double prob(int) const; // square modulus
        int N() const;
        
        // access to members
        void _set(int, Vector2);
        Vector2 _get(int) const;
        
        void _init();

        static void _register_methods();
    };

    // TODO, 2D
}
