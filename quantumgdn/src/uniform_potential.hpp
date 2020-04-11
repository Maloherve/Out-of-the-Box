#pragma once

#include "quantumsim/potentials/uniform.hpp"
#include "potential.hpp"

#include <Object.hpp>

namespace godot {
    
    template<class Coords, class Degrees>
    class uniform_potential : public potential_obj<Coords, qsim::pot::uniform<Coords>, Degrees, double> {
        typedef potential_obj<Coords, qsim::pot::uniform<Coords>, Degrees, double> Base;
        GODOT_SUBCLASS(uniform_potential, Base)

    public:
        uniform_potential(double value = 0.0) 
            : Base(value)
             {}

        ~uniform_potential() {}

        /*
         * at method
         */

        virtual double at(const Degrees&) const override {
            return this->m_ptr->get_value();
        }

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

        /*virtual Ref<potential<Coords>> _abstract() override {
            auto ptr = std::shared_ptr<qsim::potential<Coords>>(this);
            return new potential<Coords>(ptr);
        }*/

        static void _register_methods() {
            register_property<uniform_potential, double>("value", &uniform_potential::_set_value, &uniform_potential::_get_value, 0.0);
            //POTENTIAL_OBJ(uniform_potential)
        }
    };
    
    /*
     * Specializations
     */
    // 1D grid
    class grid_uniform_potential1D : public uniform_potential<size_t, double> {
        typedef uniform_potential<size_t, double> base;
        GODOT_SUBCLASS(grid_uniform_potential1D, base)
    public:
        using uniform_potential<size_t, double>::uniform_potential;
        using uniform_potential<size_t, double>::_init;

        static grid_uniform_potential1D * _new(double value) {
            auto *ptr = grid_uniform_potential1D::_new();
            ptr->_set_value(value);
            return ptr;
        }

        static void _register_methods() {}
    };

    // 2D grid
    class grid_uniform_potential2D : public uniform_potential<size_t, Vector2> {
        typedef uniform_potential<size_t, double> base;
        GODOT_SUBCLASS(grid_uniform_potential2D, base)
    public:
        using uniform_potential<size_t, Vector2>::uniform_potential;
        using uniform_potential<size_t, Vector2>::_init;

        static grid_uniform_potential2D* _new(double value) {
            auto *ptr = grid_uniform_potential2D::_new();
            ptr->_set_value(value);
            return ptr;
        }

        static void _register_methods() {}
    };
}
