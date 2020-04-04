#pragma once

#include "quantumsim/potentials/uniform.hpp"
#include "potential.hpp"

namespace godot {
    
    class uniform_potential : private qsim::pot::uniform<size_t> , public grid_potential {

        GODOT_CLASS(uniform_potential, grid_potential)
    public:
        uniform_potential();
        ~uniform_potential();
        void _init();
        static void _register_methods();
        
        // wrapper for operator()
        double access(size_t);
    };
}
