#pragma once

#include <Godot.hpp>
#include <Reference.hpp>
#include <memory>

#include <iostream>

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

    public:

        potential(std::shared_ptr<qsim::potential<Coords>> ref = nullptr) : m_pot(ref) {
            std::cerr << "Potential constructro" << std::endl;
        }

        virtual ~potential() {}
        
        // static_cast convertion convertion
        inline operator std::shared_ptr<qsim::potential<Coords>>() const {
            return std::shared_ptr<qsim::potential<Coords>>(m_pot);
        }

        inline bool is_safe() const {
            return m_pot != nullptr;
        }

        void _init() {
            // nothing to do
            m_pot = nullptr;
        }

        double access(Coords m) {
            return (*m_pot)(m);
        }

        static void _register_methods() {
            register_method("get", &potential::access);
        }
    };

    template<class Coords>
    class potential_obj : virtual public Object {
    public:

        virtual Ref<potential<Coords>> _abstract() = 0;

        operator Ref<potential<Coords>>() {
            return _abstract();
        }

#define POTENTIAL_OBJ(subclass) register_method("reference", &subclass::_abstract);
    };

    typedef potential<size_t> grid_potential;
}
