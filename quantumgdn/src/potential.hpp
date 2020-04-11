#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>
#include <memory>

#include "qgridsystem1D.hpp"

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

        void _init() {}

        void _ready() {
            // TODO, there is a better solution, general class for systems
            auto * par1 = Object::cast_to<qgridsystem1D>(get_parent());
            //auto * obj = Object::cast_as<qgridsystem2D>(get_parent());
            if (par1 != nullptr)
                par1->set_potential(this); 

            //auto * par2 = Object::cast_as<qgridsystem2D>(get_parent());
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

        potential_obj(const Args& ... args) 
            : base(nullptr) , m_ptr(std::make_shared<Specific>(args ...))
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

    typedef potential<size_t, double> grid_potential1D;
    typedef potential<size_t, Vector2> grid_potential2D;
}
