#pragma once

#include "potential_field.hpp"

#include <Curve.hpp>

namespace godot {

    class curve_field : public potential_field {
        GODOT_SUBCLASS(curve_field, potential_field)

        Ref<Curve> along_x;
        Ref<Curve> along_y;

    public:
        curve_field() = default;
        ~curve_field() = default;
        
        // override this behavior to form the scalar field
        virtual double at(const Vector2& r) const override;

        double _at(Vector2) const;

        void _init();

        static void _register_methods();
    };
}
