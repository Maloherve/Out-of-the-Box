#include "quantumsim/grid/qsystem1D.hpp"

#include <core/Godot.hpp>
#include <Node.hpp>

namespace qgdn {

    class qgridsystem1D : private qsim::grid::qsystem1D, public godot::Node {
        GODOT_CLASS(qgridsystem1D)

        Reference * m_potential;
        Reference * m_evolver;
    public:
        qgridsystem1D();
        ~qgridsystem1D();
        
        // dynamic cast to potential<size_t>
        // TODO, check if references are working
        void set_potential(godot::Reference* pot);
        void set_evolver(godot::Reference* integ);

        Reference * get_potential() const;
        Reference * get_evolver() const;

        // GD constructor
        void _init();
        
        // GD evolve
        void _process(double dt);

        // bindings
        static void _register_methods();
    };
}
