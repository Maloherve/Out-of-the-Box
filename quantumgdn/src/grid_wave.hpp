#pragma once

#include <Godot.hpp>
#include <Reference.hpp>
#include <Vector2.hpp>
#include <Array.hpp>
#include "quantumsim/grid/fw.hpp"

namespace godot {

    class grid_wave : public Reference {
        GODOT_CLASS(grid_wave, Reference)
        
        qsim::grid::wave_vector * wave_ref;
        bool to_be_deleted;

        inline const qsim::grid::wave_vector* const_wave() const {
            return const_cast<const qsim::grid::wave_vector *>(wave_ref);
        }

    public:

        grid_wave(qsim::grid::wave_vector * wave = nullptr, bool free = false);

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

        // management helpers
        void set_instance(qsim::grid::wave_vector *, bool free = false);

        inline void release() {
            to_be_deleted = false;
        }

        inline qsim::grid::wave_vector& ref() {
            return *wave_ref;
        }

        inline const qsim::grid::wave_vector& ref() const {
            return *const_wave();
        }
        
        // push elements
        //void _push(const Array&);

        /*
         * initializers
         */
        void _init();

        static void _register_methods();
    };
}
