#pragma once 

#include "debug.hpp"

namespace godot {

    template <class Coords, class Degrees>
    class potential;

    class qsimbox;
    
    template <class Coords, class Degrees>
    class qsystem : public Node {
        GODOT_CLASS(qsystem, Node)
    protected:
        qsimbox * box; 
        potential<Coords, Degrees> * m_pot;
    public:

         void set_potential(potential<Coords, Degrees> *) {
            if (pot != nullptr && _set_potential(*pot)) {
                m_pot = pot;
            }
         }

         void _ready() {
             npdebug("Ready was called")
             box = simulation_box();
         }
        
         // subclass implementation
         virtual bool _set_potential(potential<Coords, Degrees> *) { return false; }
         virtual qsimbox * simulation_box() { return nullptr; }

         static void register_methods() {
            register_method("_ready", &qgridsystem1D::_ready);
         }
    };
}
