#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>
#include <memory>

#include "qsystem.hpp"

#include "debug.hpp"

#define POTENTIAL_CLASS(instance) this->_set(instance);

namespace qsim {
    template <class Coords>
    class potential;
}

namespace godot {

    template <class Coords, class Degrees>
    class potential : public Node {
        GODOT_CLASS(potential, Node)

        qsystem<Coords, Degrees> * system;

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
        
        // default, flat potential
        virtual double at(const Degrees&) const {
            return 0;
        }

        qsystem<Coords, Degrees> * get_system() const {
            return system;
        }

        void _init() {}

        void _ready() {
            qsystem<Coords,Degrees> * parent = Object::cast_to<qsystem<Coords,Degrees>>(get_parent());
            if (parent != nullptr) {
                npdebug("System: ", parent)
                this->system = parent;
                parent->set_potential(this); 
                npdebug("Potential set")
            }
        }

        static void _register_methods() {
            register_method("get", &potential::access);
            register_method("_ready", &potential::_ready);
        }
    };

    /*
     * Wrapper for a general referenciable potential
     * Coords must be a Variant like (see Godot documentation)
     */    
    template <class Coords, class Specific, class Degrees, class ... Args>
    class potential_obj : public potential<Coords, Degrees> {
        typedef potential<Coords, Degrees> base;
        GODOT_SUBCLASS(potential_obj, base)

        static_assert(std::is_base_of<qsim::potential<Coords>, Specific>::value, "Potential specific type must be derived from qsim::potential<Coords>");

    protected:

        std::shared_ptr<Specific> m_ptr;

    public:

        potential_obj() 
            : base(nullptr) , m_ptr(std::make_shared<Specific>())
        {
            this->set_ptr(std::static_pointer_cast<qsim::potential<Coords>>(m_ptr));
        }

        potential_obj(const Args& ... args) 
            : base(nullptr) , m_ptr(std::make_shared<Specific>(args ...))
        {
            this->set_ptr(std::static_pointer_cast<qsim::potential<Coords>>(m_ptr));
        }

        void _init() {
            // nothing to do
            //m_pot = nullptr;
        }

        /*void _ready() {
            npdebug("Called")
            base::_ready();
        }*/

        virtual std::shared_ptr<qsim::potential<Coords>> get_ptr() const {
            return std::static_pointer_cast<qsim::potential<Coords>>(m_ptr);
        }

        static void _register_methods() {
            //register_method("_ready", &potential_obj::_ready);
        }
    };

    typedef potential<size_t, double> grid_potential1D;
    typedef potential<size_t, Vector2> grid_potential2D;
}
