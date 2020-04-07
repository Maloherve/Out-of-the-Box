#pragma once

#include <Godot.hpp>
#include <Reference.hpp>
#include <memory>

#include "debug.hpp"

#define POTENTIAL_CLASS(instance) this->_set(instance);

namespace qsim {
    template <class Coords>
    class potential;
}

namespace godot {

    template <class Coords>
    class potential : public Node {
        GODOT_CLASS(potential, Node)

        std::shared_ptr<qsim::potential<Coords>> m_pot;

    protected:

        void set_ptr(std::shared_ptr<qsim::potential<Coords>> ptr){
            m_pot = ptr;
        }

    public:

        potential(std::shared_ptr<qsim::potential<Coords>> ptr = nullptr) : m_pot(ptr) {
            npdebug("Potential constructed")
        }

        virtual ~potential() {}
        
        // static_cast convertion convertion
        inline operator std::shared_ptr<qsim::potential<Coords>>() const {
            return m_pot;
        }

        bool is_safe() const {
            return m_pot != nullptr;
        }

        double access(Coords m) const {
            return (*m_pot)(m);
        }

        void _init() {}

        static void _register_methods() {
            register_method("get", &potential::access);
        }
    };

    /*
     * Wrapper for a general referenciable potential
     * Coords must be a Variant like (see Godot documentation)
     */    
    template <class Coords, class Specific, class ... Args>
    class potential_obj : public potential<Coords> {
        GODOT_SUBCLASS(potential_obj, potential<Coords>)

        static_assert(std::is_base_of<qsim::potential<Coords>, Specific>::value, "Potential specific type must be derived from qsim::potential<Coords>");

    protected:

        std::shared_ptr<Specific> m_ptr;

    public:

        potential_obj(const Args& ... args) 
            : potential<Coords>(nullptr) , m_ptr(std::make_shared<Specific>(args ...))
        {
            this->set_ptr(std::static_pointer_cast<qsim::potential<Coords>>(m_ptr));
            std::cerr << "Potential constructro" << std::endl;
        }

        void _init() {
            // nothing to do
            //m_pot = nullptr;
        }

        virtual std::shared_ptr<qsim::potential<Coords>> get_ptr() const {
            return std::static_pointer_cast<qsim::potential<Coords>>(m_ptr);
        }

        static void _register_methods() {
        }
    };

    typedef potential<size_t> grid_potential;
}
