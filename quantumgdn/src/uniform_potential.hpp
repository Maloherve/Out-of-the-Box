#pragma once

#include "quantumsim/potentials/uniform.hpp"
#include "potential.hpp"

#include <Object.hpp>

namespace godot {
    
    template<class Coords>
    class uniform_potential : public potential_obj<Coords, qsim::pot::uniform<Coords>, double> {
        typedef potential_obj<Coords, qsim::pot::uniform<Coords>, double> Base;
        GODOT_SUBCLASS(uniform_potential<Coords>, Base)

    public:
        uniform_potential(double value = 0.0) 
            : Base(value)
             {}

        ~uniform_potential() {}

        /*
         * Bindings
         */

        double _get_value() const {
            return this->m_ptr->get_value();
        }

        void _set_value(double val) {
            std::cerr << "Value set: " << val << std::endl;
            return this->m_ptr->set_value(val);
        }

        /*
         * Godot setup
         */

        void _init() {
            std::cerr << "Init uniform" << std::endl;
            this->m_ptr->set_value(0.0);
        }

        static uniform_potential<Coords> * _new(double value) {
            auto * ptr = uniform_potential<Coords>::_new();
            ptr->set_value(value);
            return ptr;
        } 

        /*virtual Ref<potential<Coords>> _abstract() override {
            auto ptr = std::shared_ptr<qsim::potential<Coords>>(this);
            return new potential<Coords>(ptr);
        }*/

        static void _register_methods() {
            register_property<uniform_potential, double>("value", &uniform_potential::_set_value, &uniform_potential::_get_value, 0.0);
            //POTENTIAL_OBJ(uniform_potential)
        }
    };
    
    // alias for the grid case
    typedef uniform_potential<size_t> grid_uniform_potential;
}
