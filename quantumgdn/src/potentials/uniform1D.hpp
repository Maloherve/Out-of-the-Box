#pragma once

#include <godot/reference.hpp>

namespace qgdn {
    
    class uniform_potential : private qsim::pot::uniform<size_t> ,public godot::Reference {
        GODOT_CLASS(uniform_potential, godot::Reference)
    public:
        void _init();
        static void _register_methods();
    }
}
