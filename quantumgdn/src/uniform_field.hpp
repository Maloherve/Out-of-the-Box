#pragma once

#include "potential_field.hpp"

namespace godot {

    class TileMap;

    class uniform_field : public potential_field {
        GODOT_SUBCLASS(uniform_field, potential_field)
        
        double value;
        TileMap * tilemap;

    public:
        uniform_field();
        ~uniform_field() = default;
        
        // override this behavior to form the scalar field
        virtual double at(const Vector2&) const override;

        double _at(Vector2) const;

        void _ready();
        void _init();

        static void _register_methods();
    };
}
