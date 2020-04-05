#pragma once

#include "quantumsim/potentials/uniform.hpp"
#include "potential.hpp"

namespace godot {
    
    template<class Coords>
    class uniform_potential : private qsim::pot::uniform<Coords> , public potential<Coords> {
        GODOT_CLASS(uniform_potential, potential<Coords>)
    public:
        uniform_potential() {
            POTENTIAL_CLASS(this)
        }

        ~uniform_potential() {}

        /*
         * Bindings
         */

        double _get_value() const {
            return this->get_value();
        }

        void _set_value(double val) {
            return this->set_value(val);
        }

        /*
         * Godot setup
         */

        void _init() {
            this->set_value(0.0);
        }

        static void _register_methods() {
            register_property<uniform_potential, double>("value", &uniform_potential::_set_value, &uniform_potential::_get_value, 0.0);
        }
    };
    
    // alias for the grid case
    typedef uniform_potential<size_t> grid_uniform_potential;
}
