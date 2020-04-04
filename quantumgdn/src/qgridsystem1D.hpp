#include "quantumsim/grid/qsystem1D.hpp"

#include <core/Godot.hpp>
#include <Reference.hpp>

namespace qgdn {

    class qgridsystem1D : private qsim::grid::qsystem1D, public godot::GodotScript<Reference> {
        GODOT_CLASS(qgridsystem1D)
    public:
        // constructor?
        void _init();

        // bindings
        static void _register_methods();
    };
}
