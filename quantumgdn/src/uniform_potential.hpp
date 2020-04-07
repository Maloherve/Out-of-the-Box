#pragma once

#include "quantumsim/potentials/uniform.hpp"
#include "potential.hpp"

#include <Object.hpp>

#include <iostream>

namespace godot {
    
    template<class Coords>
    class uniform_potential : public qsim::pot::uniform<Coords>, public potential_obj<Coords> {
        GODOT_CLASS(uniform_potential, Object)
    public:
        uniform_potential(double value = 0.0) : qsim::pot::uniform<Coords>(value) {}

        ~uniform_potential() {}

        /*
         * Bindings
         */

        double _get_value() const {
            return this->get_value();
        }

        void _set_value(double val) {
            std::cerr << "Value set: " << val << std::endl;
            return this->set_value(val);
        }

        /*
         * Godot setup
         */

        void _init() {
            std::cerr << "Init uniform" << std::endl;
            this->set_value(0.0);
        }

        virtual Ref<potential<Coords>> _abstract() override {
            auto ptr = std::shared_ptr<qsim::potential<Coords>>(this);
            return new potential<Coords>(ptr);
        }

        static void _register_methods() {
            register_property<uniform_potential, double>("value", &uniform_potential::_set_value, &uniform_potential::_get_value, 0.0);
            POTENTIAL_OBJ(uniform_potential)
        }
    };
    
    // alias for the grid case
    typedef uniform_potential<size_t> grid_uniform_potential;
}
