#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <Vector2.hpp>
#include "quantumsim/grid/fw.hpp"

namespace godot {

    class wave_packet;

    class grid_wave : public Node {
        GODOT_CLASS(grid_wave, Node)
        
        qsim::grid::wave_vector * wave_ref;

        inline const qsim::grid::wave_vector* const_wave() const {
            return const_cast<const qsim::grid::wave_vector *>(wave_ref);
        }

    public:

        grid_wave(qsim::grid::wave_vector * wave = nullptr);

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
        void set_instance(qsim::grid::wave_vector*);

        void move_into(qsim::grid::wave_vector&);

        inline qsim::grid::wave_vector& ref() {
            return *wave_ref;
        }


        inline const qsim::grid::wave_vector& ref() const {
            return *const_wave();
        }
        
        /*
         * Push a wave packet
         */
        //void _set_packet(Ref<wave_packet>);
        
        // push elements
        //void _push(const Array&);

        /*
         * initializers
         */
        void _init();

        static void _register_methods();
    };
}
