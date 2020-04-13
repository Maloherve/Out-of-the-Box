#pragma once 

#include <Godot.hpp>
#include <Node.hpp>
#include "debug.hpp"

namespace godot {

    template <class Coords, class Degrees>
    class potential;

    class qsimbox;
    
    template <class Coords, class Degrees>
    class qsystem : public Node{
        GODOT_CLASS(qsystem, Node)
    protected:
        potential<Coords, Degrees> * m_pot;
    public:

         qsystem() {
             npdebug("Constructing system")
         }

         virtual ~qsystem() {
             npdebug("Destructing system")
         }

         // subclass implementation
         virtual bool _set_potential(potential<Coords, Degrees> *) { return false; }
         virtual qsimbox * box() const { return nullptr; }

         //virtual bool _set_potential(potential<Coords, Degrees> *) = 0;
         //virtual qsimbox * box() const = 0;

         void set_potential(potential<Coords, Degrees> * _pot) {
            npdebug("Call")
            if (_pot != nullptr) 
            { 
                npdebug("Before the dafuck")
                if (_set_potential(_pot)) {
                    m_pot = _pot;
                }
                npdebug("After the dafuck")
            }
         }

         /*void _ready() {
             npdebug("Ready was called")
             box = simulation_box();
         }*/

         void _init() {
         }
        
         // convertion from Coordinate space into Degrees space
         virtual Degrees to_degrees(const Coords&) const {
             return Degrees();
         }

         static void _register_methods() {
            //register_method("_ready", &qsystem::_ready);
         }
    };

    typedef qsystem<size_t,double> qgridsystem1D_base;
}
