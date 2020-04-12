#pragma once

#include "quantumsim/grid/qsystem1D.hpp"

#include <Godot.hpp>
#include <Node.hpp>
#include <Reference.hpp>

#include "wave_init1D.hpp"

#include "qsystem.hpp"
#include "potential.hpp"

namespace godot {

    class grid_wave1D;

    class qgridsystem1D : private qsim::grid::qsystem1D, public qgridsystem1D_base {
        GODOT_SUBCLASS(qgridsystem1D, qgridsystem1D_base)
        
        grid_wave1D * _wave;

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
        virtual qsimbox * box() const override;

        /*
         * size
         */

        size_t N() const;

        /*
         * Position indexing
         */

        double x(size_t) const;

        /*
         *  hbar
         */

        void set_hbar(double);
        double get_hbar() const;

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
