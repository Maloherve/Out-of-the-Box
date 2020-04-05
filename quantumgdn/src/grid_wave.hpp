#pragma once

#include <Godot.hpp>
#include <Reference.hpp>
#include <Vector2.hpp>
#include <Array.hpp>

#include "quantumsim/grid/wave.hpp"

namespace godot {

    class grid_wave : public qsim::grid::wave_vector, public Object {
        GODOT_CLASS(grid_wave, Object)
    public:

        grid_wave();
        ~grid_wave();
        
        /*
         * Methods to be bounded
         */
        double real(int) const;
        double imag(int) const;
        double prob(int) const; // square modulus
        size_t _size() const;
        
        // access to members
        void _set(size_t, Vector2);
        Vector2 _get(int) const;
        
        // push elements
        //void _push(const Array&);

        /*
         * initializers
         */
        void _init();

        static void _register_methods();
    };
}
