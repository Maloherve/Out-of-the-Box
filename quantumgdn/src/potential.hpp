#pragma once

#include <Godot.hpp>
#include <Reference.hpp>
#include <memory>

#define POTENTIAL_CLASS(instance) this->_set(instance);

namespace qsim {
    template <class Coords>
    class potential;
}

namespace godot {

    /*
     * Wrapper for a general referenciable potential
     */    
    template <class Coords>
    class potential : public Reference {
        GODOT_CLASS(potential<Coords>, Reference)

        std::shared_ptr<qsim::potential<Coords>> m_pot;

    protected:
        void _set(qsim::potential<Coords>* pot) {
            m_pot = std::shared_ptr<qsim::potential<Coords>>(pot);
        }

    public:

        potential() : m_pot(nullptr) {}

        virtual ~potential() {}
        
        // static_cast convertion convertion
        inline operator std::shared_ptr<qsim::potential<Coords>>() const {
            return m_pot;
        }

        inline bool is_safe() const {
            return m_pot != nullptr;
        }

        void _init() {
            // nothing to do
        }

        double access(Coords m) {
            return (*m_pot)(m);
        }

        static void _register_methods() {
            register_method("get", &potential::access);
        }
    };

    typedef potential<size_t> grid_potential;
}
