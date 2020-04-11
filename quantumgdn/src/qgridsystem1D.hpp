#pragma once

#include "quantumsim/grid/qsystem1D.hpp"

#include <Godot.hpp>
#include <Node.hpp>
#include <Reference.hpp>

#include "wave_init1D.hpp"
#include "grid_wave.hpp"

#include "qsystem.hpp"
#include "potential.hpp"

namespace godot {

    class qgridsystem1D : private qsim::grid::qsystem1D, public qgridsystem1D_base {
        GODOT_SUBCLASS(qgridsystem1D, qgridsystem1D_base)
        
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
        virtual bool _set_potential(grid_potential1D *) override;
        virtual qsimbox * simulation_box() override;

        /*
         * Boundary properties depend on potential
         */

        //void set_evolver(Reference* integ);
        //Reference * get_evolver() const;
        
        /*
         * Access to wave function
         */

        void set_wave(Ref<wave_init1D>);

        // GD constructor
        void _init();
        void _ready();
        
        // GD evolve
        void _fixed_process(double dt);

        // bindings
        static void _register_methods();
    };
}
