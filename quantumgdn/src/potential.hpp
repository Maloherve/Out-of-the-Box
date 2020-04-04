#pragma once

#include <core/Godot.hpp>
#include <Reference.hpp>
#include <memory>
#include "quantumsim/potential.hpp"

#define POTENTIAL_CLASS(instance, Coords) _set(instance);

namespace godot {
    
    template <class Coords>
    class potential : public Reference {
        GODOT_CLASS(potential, Reference)

        std::shared_ptr<qsim::potential<Coords>> m_pot;

    protected:
        void _set(qsim::potential<Coords>* pot) {
            m_pot = std::shared_ptr<qsim::potential<Coords>>(pot);
        }

    public:

        potential() : m_pot(nullptr) {}

        virtual ~potential() {}

        std::shared_ptr<qsim::potential<Coords>> get_ptr() {
            return m_pot;
        }

        void _init() {}

        static void _register_methods() {}
    };

    typedef potential<size_t> grid_potential;
}
