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

    /*class grid1D : public Area2D {
        GODOT_CLASS(grid1D, Area2D)
    public:
        using Area2D::Area2D;
        
        size_t to_index(double);
        double to_space(size_t);
    };

    class grid2D : public Area2D {
        GODOT_CLASS(grid2D, Area2D)
    public:
        using Area2D::Area2D;
        
        size_t to_index(double, double);
        size_t to_index(const Vector2&);
        Vector2 to_space(size_t);
        Vector2 to_space(size_t, size_t);
    };*/
}
