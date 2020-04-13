#pragma once

#include <Godot.hpp>
#include <Node.hpp>

namespace godot {

    class potential_body : public Node {
        GODOT_CLASS(potential_body, Node)
    public:
        potential_body() = default;
        ~potential_body() = default;

        void _init();

        static void _register_methods();
    };
}
