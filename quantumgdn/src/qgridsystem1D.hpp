#include "quantumsim/grid/qsystem1D.hpp"

#include <core/Godot.hpp>
#include <Node.hpp>
#include "potential.hpp"

namespace godot {

    class qgridsystem1D : private qsim::grid::qsystem1D, public Node {
        GODOT_CLASS(qgridsystem1D, Node)

        potential<size_t> * m_potential;
        //Reference * m_evolver;

    public:
        qgridsystem1D();
        ~qgridsystem1D();

        using qsim::grid::qsystem1D::energy;
        using qsim::grid::qsystem1D::position;
        using qsim::grid::qsystem1D::momentum;
        using qsim::grid::qsystem1D::normalize;
        using qsim::grid::qsystem1D::map;
        using qsim::grid::qsystem1D::size;
        
        // dynamic cast to potential<size_t>
        // TODO, check if references are working
        void set_potential(potential<size_t>* pot);
        //void set_evolver(Reference* integ);

        potential<size_t> * get_potential() const;
        //Reference * get_evolver() const;

        // GD constructor
        void _init();
        
        // GD evolve
        void _process(double dt);

        // bindings
        static void _register_methods();
    };
}
