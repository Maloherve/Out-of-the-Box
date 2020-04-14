#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>

namespace godot {

    class potential_field : public Node2D {
        GODOT_CLASS(potential_field, Node2D)
    public:
        potential_field() = default;
        virtual ~potential_field() = default;
        
        // override this behavior to form the scalar field
        virtual double at(const Vector2&) const {
            return 0.0;
        }

        void _init() {}

        static void _register_methods() {}
    };
}
