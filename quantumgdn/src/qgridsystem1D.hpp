#pragma once

#include "quantumsim/grid/qsystem1D.hpp"

#include <Godot.hpp>
#include <Node.hpp>
#include <Reference.hpp>

#include "wave_init1D.hpp"

#include "gdqsystem.hpp"

namespace godot {

    class grid_wave1D;

    class qgridsystem1D : public gdqsystem, private qsim::grid::qsystem1D {
        GODOT_SUBCLASS(qgridsystem1D, gdqsystem)
        
        grid_wave1D * _wave;

        int steps_per_tick;
        double dt_gain;

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
        double _probability(double,double) const;

        virtual Vector2 location(size_t) const override;

        /*
         * size
         */

        int N() const;

        /*
         * Position indexing
         */

        double x(int) const;

        /*
         * steps per units of time
         */

        int get_steps_per_tick() const;
        void set_steps_per_tick(int);

        /*
         *  hbar
         */

        void set_hbar(double);
        double get_hbar() const;

        /*
         * access to the potential
         */

        double potential_at(double x) const;
        double potential_at_index(size_t k) const;

        /*
         * Delta time amplification
         */

        void set_dt_gain(double);
        double get_dt_gain() const;

        /*
         * Boundary properties depend on potential
         */

        //void set_evolver(Reference* integ);
        //Reference * get_evolver() const;
        
        /*
         * Access to wave function
         */
        void set_wave(Ref<wave_init1D>);
        grid_wave1D * get_wave() const;
        Vector2 psi(int) const;
        double psi_norm(int) const;
        double psi_real(int) const;
        double psi_imag(int) const;

        // GD constructor
        void _init();
        void _ready();
        
        // GD evolve
        void _fixed_process(double dt);

        // bindings
        static void _register_methods();
    };
}
