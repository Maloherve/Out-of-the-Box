#pragma once

#include "quantumsim/grid/qsystem1D.hpp"

#include <Godot.hpp>
#include <Node.hpp>
#include <Reference.hpp>

#include "wave_init1D.hpp"

#include "grid_wave.hpp"

#include "qsystem.hpp"

namespace godot {

    class grid_potential1D;
    class qsimbox;

    class qgridsystem1D : private qsim::grid::qsystem1D, public Node {
        GODOT_SUBCLASS(qgridsystem1D, Node)
        
        qsimbox * box; 
        grid_potential1D * m_pot;

    public:
        qgridsystem1D();
        ~qgridsystem1D();

        /*
         * Positioning
         */

        /*
         * Methods declarations
         */

        double _energy() const;
        double _position() const;
        double _momentum() const;

        /*
         * Non-binded access to potential
         */
        grid_potential1D * set_potential(grid_potential1D *);

        /*
         * Boundary properties depend on potential
         */

        //void set_evolver(Reference* integ);
        //Reference * get_evolver() const;
        
        /*
         * Access to wave function
         */

        //void _set_wave(grid_wave *);
        //grid_wave * _get_wave() const;
        //
        void set_wave(Ref<wave_init1D>, double);

        // GD constructor
        void _init();
        void _ready();
        
        // GD evolve
        void _fixed_process(double dt);

        // bindings
        static void _register_methods();
    };
}
