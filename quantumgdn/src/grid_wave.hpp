#pragma once

#include <Godot.hpp>
#include <Reference.hpp>
#include <Vector2.hpp>

namespace qsim::grid {
    class wave_vector;
}

namespace godot {

    class grid_wave : public Reference {
        GODOT_CLASS(grid_wave, Reference)
        
        // pointer to the wave function
        qsim::grid::wave_vector * m_wave;

    public:

        grid_wave();
        ~grid_wave();
        
        /*
         * Methods to be bounded
         */
        double real(int) const;
        double imag(int) const;
        double prob(int) const; // square modulus
        size_t size() const;
        
        // access to members
        void set(size_t, Vector2);
        Vector2 get(int) const;

        /*
         * Helper methods
         */

        void _set(qsim::grid::wave_vector *wave) {
            m_wave = wave;
        }
        
        // move the pointer
        /*grid_wave& operator=(const grid_wave& other) {
            m_wave = other.m_wave;
            return *this;
        }*/
        
        /*
         * initializers
         */
        void _init();

        static void _register_methods();
    };
}
