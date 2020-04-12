#pragma once 

#include <Godot.hpp>
#include <Node.hpp>
#include "debug.hpp"

namespace godot {

    template <class Coords, class Degrees>
    class potential;

    class qsimbox;
    
    template <class Coords, class Degrees>
    class qsystem : public Node {
        GODOT_CLASS(qsystem, Node)
    protected:
        potential<Coords, Degrees> * m_pot;
    public:

         void set_potential(potential<Coords, Degrees> * pot) {
            if (pot != nullptr && _set_potential(pot)) {
                m_pot = pot;
            }
         }

         /*void _ready() {
             npdebug("Ready was called")
             box = simulation_box();
         }*/

         void _init() {
         }
        
         // subclass implementation
         virtual bool _set_potential(potential<Coords, Degrees> *) { return false; }
         virtual qsimbox * box() const { return nullptr; }

         static void _register_methods() {
            //register_method("_ready", &qsystem::_ready);
         }
    };

    typedef qsystem<size_t,double> qgridsystem1D_base;
}