#pragma once

#include "quantumsim/grid/qsystem1D.hpp"

#include <Godot.hpp>
#include <Node.hpp>
#include <Reference.hpp>
#include "potential.hpp"

#include "grid_wave.hpp"

namespace godot {

    class qbounds1D : public Reference {
        GODOT_CLASS(qbounds1D, Reference)

        qsim::grid::qsystem1D::bound * ptr;

    public:
        qbounds1D(qsim::grid::qsystem1D::bound *_ptr = nullptr);
        ~qbounds1D() = default;

        void set_pos(double _pos);
        double get_pos() const;

        void set_mode(int _mode);
        int get_mode() const;

        void _init() {}

        inline void set_instance(qsim::grid::qsystem1D::bound* _ptr) {
            ptr = _ptr;
        }

        static void _register_methods();
    };

    class qgridsystem1D : private qsim::grid::qsystem1D, public Node {
        GODOT_CLASS(qgridsystem1D, Node)

        // implicit child, accessible by psi
        grid_wave * wave_fct;

        // explicit child, accessible by V
        // it's supposed to be an external object
        grid_potential * m_potential;

        Ref<qbounds1D> low_bound, up_bound;
        
    public:
        qgridsystem1D();
        ~qgridsystem1D();
        
        /*
         * Methods declarations
         */

        double _energy() const;
        double _position() const;
        double _momentum() const;
        double _normalize();
        double _map(size_t) const;
        double _size() const;

        /*
         * Potential property
         */
        void _set_potential(grid_potential * pot);
        grid_potential * _get_potential() const;

        //void set_evolver(Reference* integ);
        //Reference * get_evolver() const;
        
        /*
         * Access to wave function
         */

        void _set_wave(grid_wave *);
        grid_wave * _get_wave() const;

        /*
         * Access to boundaries
         */

        void _set_upper_bound(Ref<qbounds1D>);
        void _set_lower_bound(Ref<qbounds1D>);

        Ref<qbounds1D> _get_upper_bound() const;
        Ref<qbounds1D> _get_lower_bound() const;

        // GD constructor
        void _init();
        void _ready();
        
        // GD evolve
        void _fixed_process(double dt);

        // bindings
        static void _register_methods();
    };
}
