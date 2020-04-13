#pragma once

#include <Godot.hpp>
#include <Area2D.hpp>

namespace godot {

    class RectangleShape2D;
    class CollisionShape2D;

    class qsimbox : public Area2D {
        GODOT_CLASS(qsimbox, Area2D)
        
        RectangleShape2D * rect;
        CollisionShape2D * shape;

    public:
        using Area2D::Area2D;

        double x() const;
        double y() const;

        void set_width(double);
        double width() const;

        void set_height(double);
        double height() const;

        void _init();
        void _ready();

        static void _register_methods();
    };
}
