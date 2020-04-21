#pragma once 

#include <Godot.hpp>
#include <Node2D.hpp>

#include <memory>

namespace godot {
    
    class curve_potential;
    
    class gdqsystem : public Node2D {
        GODOT_CLASS(gdqsystem, Node2D)
    private:

         std::shared_ptr<curve_potential> m_pot;

         double width;
         double height;

    public:
         gdqsystem(std::shared_ptr<curve_potential> ptr = nullptr);
         virtual ~gdqsystem() = default;

         virtual Vector2 location(size_t) const;

         std::shared_ptr<curve_potential> potential() const;

         /*
          * size accessors
          */
        
         void set_width(double);
         double get_width() const;

         void set_height(double);
         double get_height() const;

         void set_size(Vector2);
         Vector2 get_size() const;

         /*
          * Potential buffering state access
          */

         void set_potential_buffering(bool);
         bool get_potential_buffering() const;

         /*
          * GDnative setup with 
          * Entering CollisionObject2D auto-detection
          */

         void _init();
         void _ready();

         void on_body_entered(Node * entry);
         void on_body_exited(Node * entry);

         static void _register_methods();
    };
}
